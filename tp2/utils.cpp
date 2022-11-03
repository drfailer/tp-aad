#include "utils.h"
#include <cstdlib>
#include <stdlib.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

int T[k] = {0};

/**
 * @brief calcul du hash pour une solution
 *
 * @param solution Solution pour laquelle on doit calculer le hash.
 * @param instance Instance du graph � laquelle la solution appartient.
 *
 * @return hash de la solution.
 */
static inline int hachage(t_solution &solution, t_instance &instance) {
  unsigned long int h = 0;
  unsigned long int dateCourrante;

  for (int i = 1; i <= instance.nbPieces; ++i) {
    for (int j = 1; j <= instance.nbMachines; ++j) {
      dateCourrante = solution.dates[i][j];
      h = (h + (dateCourrante * dateCourrante)) % k;
    }
  }

  if (h > k)
    exit(-1);
  return h;
}

/**
 * @brief Permet d'initialiser un instance d'un graphe sauvegard�e dans un
 *        fihier.
 *
 * @param instance Instance � initialiser.
 * @param name Nom du fichier � charger.
 */
void lectureFichier(t_instance &instance, std::string name) {
  std::ifstream fichier(name, std::ios::in);
  int           numMachine;
  int           machine;

  if (fichier) {
    fichier >> instance.nbPieces >> instance.nbMachines;

    for (int i = 1; i <= instance.nbPieces; ++i) {
      for (int j = 1; j <= instance.nbMachines; ++j) {
        fichier >> machine;
        instance.machines[i][j] = machine + 1; // on compte � partir de 1
        fichier >> instance.coutPieceMachine[i][j];
      }
    }

    fichier.close();
  } else {
    std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Permet de g�n�rer al�atoirement un vecteur de bierwith pour une
 * instance d'un graphe. Modifie le tableau `bierwith` de `solution`.
 *
 * @param solution Solution pour laquelle on g�n�re un nouveau vecteur.
 * @param instance Instance du graphe pour laquelle on g�n�re le vecteur.
 */
void genererVecteurBierwith(t_solution &solution, t_instance &instance) {
  int stockPieces[nmax];
  int stockPiecesNumeros[nmax];
  int nbPieces = instance.nbPieces;
  int machineCourrante;

  // intialisation du stock de piece
  for (int i = 1; i <= instance.nbPieces; ++i) {
    stockPiecesNumeros[i] = i;
    stockPieces[i] = instance.nbMachines;
  }

  // on vide le stock de pi�ce en tirant un num�ro al�atoirement et on met la
  // pi�ce tir�e dans le vecteur. A la fin, le tableau contient bien la bonne
  // quantit� de chaque pi�ces.
  for (int i = 1; i <= instance.nbPieces * instance.nbMachines; ++i) {
    machineCourrante = rand() % nbPieces + 1;

    stockPieces[machineCourrante]--;
    solution.bierwith[i] = stockPiecesNumeros[machineCourrante];
    if (stockPieces[machineCourrante] == 0) {
      stockPieces[machineCourrante] = stockPieces[nbPieces];
      stockPiecesNumeros[machineCourrante] = stockPiecesNumeros[nbPieces--];
    }
  }
}

/**
 * @brief Evalue une solution pour une instance de graphe et un vecteur de
 * bierwith. L'�valuation met � jour la solution en modifiant le tableau des
 * p�res, des dates, ... Le hash de la solution trouv�e est calcul� � la fin.
 *
 * @param solution Solution que l'on met � jour lors de l'�valuation. Elle
 *        contient le vecteur de bierwith qui determine le sens dans lequel on
 *        traite les noeuds du graphe.
 * @param instance Instance du graphe pour laquelle on evalue la solution.
 */
void evaluer(t_solution &solution, t_instance &instance) {
  int occurencePiece[nmax] = {0};
  int ordreSurMachine[nmax][2] = {0};
  int pieceActuelle;
  int rangPiece = 0;
  int oldDate;
  int machineCourrante;
  int numeroPieceMachine;
  int rangPieceMachine;

  // reset des �l�ments de la solution
  solution.count = 0;
  memset(&solution.dates[0][0], 0, (nmax * nmax) * sizeof(int));
  memset(&solution.pere[0][0], 0, (nmax * nmax) * sizeof(int));

  // parcours du vecteur de bierwith et mise � jour de solution
  for (int i = 1; i <= instance.nbMachines * instance.nbPieces; ++i) {
    pieceActuelle = solution.bierwith[i];
    // pour connaitre le rang d'une pi�ce, ie le num�ro du lien horizontal, il
    // faut compter le nombre de fois que cette pi�ce � �t� trouv�e dans le
    // vecteur de bierwith.
    rangPiece = ++occurencePiece[pieceActuelle];

    // lien horizontal
    if (occurencePiece[pieceActuelle] > 1) {
      oldDate = solution.dates[pieceActuelle][rangPiece - 1]; // date du noeud
                                                              // pr�c�dent
                                                              // (horizontalement)

      if (oldDate + instance.coutPieceMachine[pieceActuelle][rangPiece - 1] >
          solution.dates[pieceActuelle][rangPiece]) {
        solution.dates[pieceActuelle][rangPiece] =
            oldDate + instance.coutPieceMachine[pieceActuelle][rangPiece - 1];
        solution.pere[pieceActuelle][rangPiece].rang = rangPiece - 1;
        solution.pere[pieceActuelle][rangPiece].piece = pieceActuelle;
      }
    }

    machineCourrante = instance.machines[pieceActuelle][rangPiece];

    // arc vertical
    if (ordreSurMachine[machineCourrante][0] != 0) {
      numeroPieceMachine = ordreSurMachine[machineCourrante][0];
      rangPieceMachine = ordreSurMachine[machineCourrante][1];
      oldDate = solution.dates[numeroPieceMachine][rangPieceMachine];

      if (oldDate +
              instance.coutPieceMachine[numeroPieceMachine][rangPieceMachine] >
          solution.dates[pieceActuelle][rangPiece]) {
        solution.dates[pieceActuelle][rangPiece] =
            oldDate +
            instance.coutPieceMachine[numeroPieceMachine][rangPieceMachine];
        solution.pere[pieceActuelle][rangPiece].rang = rangPieceMachine;
        solution.pere[pieceActuelle][rangPiece].piece = numeroPieceMachine;
      }
    }

    // mise � jour de l'ordre de passage sur les machines
    ordreSurMachine[machineCourrante][0] = pieceActuelle;
    ordreSurMachine[machineCourrante][1] = rangPiece;

    // si on traite le dernier noeud (*), on met � jour le cout de la solution
    // et le p�re du dernier noeud.
    if (rangPiece == instance.nbMachines) {
      if (solution.dates[pieceActuelle][rangPiece] +
              instance.coutPieceMachine[pieceActuelle][rangPiece] >
          solution.count) {
        solution.count = solution.dates[pieceActuelle][rangPiece] +
                         instance.coutPieceMachine[pieceActuelle][rangPiece];
        solution.pereEtoile.piece = pieceActuelle;
        solution.pereEtoile.rang = rangPiece;
      }
    }
  }
  solution.h = hachage(solution, instance);
}

/**
 * @brief Recherche des indices de 2 �l�ments dans le vecteur de bierwith.
 */
void recherchePosition(t_solution &solution, t_instance &instance,
                       int pieceCour, int rangCour, int rangPred, int piecePred,
                       int &indexCour, int &indexPred) {
  bool predTrouve = false;
  int  rangICour = instance.nbMachines;
  int  rangIPred = instance.nbMachines;

  for (int i = instance.nbMachines * instance.nbPieces; !predTrouve && i >= 1;
       --i) {
    if (solution.bierwith[i] == pieceCour) {
      if (rangICour == rangCour)
        indexCour = i;
      rangICour -= 1;
    }
    if (solution.bierwith[i] == piecePred) {
      if (rangIPred == rangPred) {
        indexPred = i;
        predTrouve = true;
      }
      rangIPred -= 1;
    }
  }
}

/**
 * @brief Permute les valeurs au indices i et j dans le vecteur de bierwith.
 */
static inline void permuter(t_solution &solution, int i, int j) {
  int c = solution.bierwith[i];
  solution.bierwith[i] = solution.bierwith[j];
  solution.bierwith[j] = c;
}

/**
 * @brief Recherche du minimum local par rapport � une solution donn�e.
 *
 * @param solution Solution � partir de laquelle on cherche un minimum local.
 * @param instance Instance du graphe de la solution.
 * @param maxIter  Nombre d'it�ration maximum pour �viter une boucle infinie.
 */
void rechercheLocale(t_solution &solution, t_instance &instance, int maxIter) {
  int        iter = 1;
  int        stop = 0;
  int        rangCour, pieceCour, rangPred, piecePred;
  int        indexCour, indexPred;
  t_solution solutionPrime;
  int        machineCour, machinePred;
  /* int solutionVisite[k] = {0}; */

  evaluer(solution, instance);
  // noeud courant = *
  rangCour = -1;
  pieceCour = -1;
  machineCour = -1;
  // pere de *
  rangPred = solution.pereEtoile.rang;
  piecePred = solution.pereEtoile.piece;
  machinePred = instance.machines[piecePred][rangPred];
  while ((iter < maxIter) && (stop == 0)) {
    // test m�me machine
    if (machineCour == machinePred) {
      solutionPrime = solution;
      // modification de `solutionPrime`
      recherchePosition(solution, instance, pieceCour, rangCour, rangPred,
                        piecePred, indexCour, indexPred);
      permuter(solutionPrime, indexCour, indexPred);
      // evaluation de la nouvelle solution
      evaluer(solutionPrime, instance);

      // si on am�liore la solution
      if (solutionPrime.count < solution.count) { // TODO: use the hash to
                                                  // test if the solutionPrime
                                                  // is known
        solution = solutionPrime;
        // noeud courant = *
        rangCour = -1;
        pieceCour = -1;
        machineCour = -1;
        // pere de *
        rangPred = solution.pereEtoile.rang;
        piecePred = solution.pereEtoile.piece;
        machinePred = instance.machines[piecePred][rangPred];
      } else {
        // i = j
        rangCour = rangPred;
        pieceCour = piecePred;
        machineCour = machinePred;
        // j = pere[i]
        rangPred = solution.pere[pieceCour][rangCour].rang;
        piecePred = solution.pere[pieceCour][rangCour].piece;
        machinePred = instance.machines[piecePred][rangPred];
      }
    } else {
      // i = j
      rangCour = rangPred;
      pieceCour = piecePred;
      machineCour = machinePred;
      // j = pere[i]
      rangPred = solution.pere[pieceCour][rangCour].rang;
      piecePred = solution.pere[pieceCour][rangCour].piece;
      machinePred = instance.machines[piecePred][rangPred];
    }

    // arret de la boucle
    if (rangCour == 0 || iter == maxIter) {
      stop = 1;
    }
    iter++;
  }
}

/**
 * @brief G�n�re un voisin d'un vecteur de bierwith en permutant 2 indices dans
 *        un vecteur donn�.
 *
 * @param solution Solution qui contient un vecteur de bierwith.
 * @param instance Instance du graphe de solution.
 *
 * @return Solution voisinne de la solution donn�e en entr�e.
 */
t_solution genererVoisin(t_solution &solution, t_instance &instance) {
  t_solution voisin = solution;
  int        tailleBierwith = instance.nbPieces * instance.nbMachines;
  int        p1 = rand() % tailleBierwith + 1;
  int        p2;

  do {
    p2 = rand() % tailleBierwith + 1;
  } while (solution.bierwith[p1] == solution.bierwith[p2]);

  permuter(voisin, p1, p2);

  return voisin;
}

/**
 * @brief Recherche du minimum global. G�n�re un certain nombre de vecteur de
 *        bierwith. Pour la meilleur solution, g�n�re un certain nombre de
 *        voisins, et effectue une recherche locale pour chacun d'eux. L'�tape
 *        pr�c�dente est effectu�e un certain nombre de fois sur le meilleur
 *        voisin trouv�.
 *        A la fin solution contient la meilleure solution trouv�e.
 *
 * @param solution Solution trouv�
 * @param instance Instance du graphe pour laquelle on cherche une solution.
 * @param nbELS    Nombre de fois o� on g�n�re des voisins et on r�cup�re le
 *                 meilleur.
 * @param nbVoisin Nombre de voisin � g�n�rer � chaque �tape
 * @param nbIter   Nombre d'it�ration de l'algorithme
 */
void grasp(t_solution &solution, t_instance &instance, int nbELS, int nbVoisin,
           int nbIter) {
  t_solution voisin;
  t_solution bestOfTheBestOfTheBestOfTheBestSir;
  bestOfTheBestOfTheBestOfTheBestSir.count = inf;
  int        iter = 0;

  memset(&T[0], k, sizeof(int));

  for (int i = 1; i <= nbIter; i++) {
    /* etape 1. Choisir une solution non trait�e au hasard */
    do {
      genererVecteurBierwith(solution, instance);
      evaluer(solution, instance);
      rechercheLocale(solution, instance, 1000000);
    } while (T[solution.h] == 1);
    T[solution.h] = 1; // marquage de la solution

    t_solution sol_depart_els = solution;

    /* etape 2. G�n�rer `nbVoisin` voisins, garder le meilleur et recommencer
     *          `nbELS` fois.
     */
    for (int j = 1; j <= nbELS; j++) {
      int        nbv = 0;
      t_solution best;
      best.count = inf;

      // g�n�ration des voisins de la solution de d�part + sauvegarde du
      // meilleur
      while ((nbv < nbVoisin && iter < 1000) || nbv == 0) {
        voisin = genererVoisin(sol_depart_els, instance);
        evaluer(voisin, instance);
        rechercheLocale(voisin, instance, 1000000);
        if (T[voisin.h] == 0) {
          if (voisin.count < best.count)
            best = voisin;
          nbv++;
          T[voisin.h] = 1;
        }
        iter++;
      }
      sol_depart_els = best; // on repart de la meilleur solution

      // sauvegarde du meilleur
      if (best.count < bestOfTheBestOfTheBestOfTheBestSir.count)
        bestOfTheBestOfTheBestOfTheBestSir = best;
    }
  }
  solution = bestOfTheBestOfTheBestOfTheBestSir;
}

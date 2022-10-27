#include "utils.h"
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

static inline int hachage(t_solution& solution, t_instance& instance) {
    unsigned long int h = 0;
    unsigned long int dateCourrante;

    for (int i = 1; i <= instance.nbPieces; ++i) {
        for (int j = 1; j <= instance.nbMachines; ++j) {
            dateCourrante = solution.dates[i][j];
            h += (dateCourrante * dateCourrante) % k;
        }
    }

    return h;
}

void lectureFichier(t_instance& instance, std::string name)
{
    std::ifstream fichier(name, std::ios::in);
    int numMachine;
    int machine;

    if (fichier)
    {
        fichier >> instance.nbPieces >> instance.nbMachines;

        for (int i = 1; i <= instance.nbPieces; ++i) {
            for (int j = 1; j <= instance.nbMachines; ++j) {
                fichier >> machine;
				instance.machines[i][j] = machine + 1; // on compte à partir de 1
                fichier >> instance.coutPieceMachine[i][j];
            }
        }

        fichier.close();
    }
    else
        std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
}

void genererVecteurBierwith(t_solution& solution, t_instance& instance)
{
    int stockPieces[nmax];
    int stockPiecesNumeros[nmax];
    int nbPieces = instance.nbPieces;
    int machineCourrante;


    // intialisation
    for (int i = 1; i <= instance.nbPieces; ++i) {
        stockPiecesNumeros[i] = i;
        stockPieces[i] = instance.nbMachines;
    }

    for (int i = 1; i <= instance.nbPieces * instance.nbMachines; ++i) {
        machineCourrante = rand()%nbPieces + 1;

		stockPieces[machineCourrante]--;
        solution.bierwith[i] = stockPiecesNumeros[machineCourrante];
        if (stockPieces[machineCourrante] == 0) {
            stockPieces[machineCourrante] = stockPieces[nbPieces];
			stockPiecesNumeros[machineCourrante] = stockPiecesNumeros[nbPieces--];
        }
    }
}

void evaluer(t_solution& solution, t_instance& instance)
{
    int occurencePiece[nmax] = { 0 };
    int ordreSurMachine[nmax][2] = { 0 };
    int pieceActuelle;
    int rangPiece = 0;
    int oldDate;
    int nouveauCout, ancientCout;
    int machineCourrante;
    int numeroPieceMachine;
    int rangPieceMachine;

    solution.count = 0;
    memset(&solution.dates[0][0], 0, (nmax*nmax)*sizeof(int));
    memset(&solution.pere[0][0], 0, (nmax*nmax)*sizeof(int));

    for (int i = 1; i <= instance.nbMachines * instance.nbPieces; ++i) { // parcours vecteur Bierwith
        pieceActuelle = solution.bierwith[i];
        rangPiece = ++occurencePiece[pieceActuelle];

        // lien horizontal
        if (occurencePiece[pieceActuelle] > 1) {
            oldDate = solution.dates[pieceActuelle][rangPiece - 1];

            nouveauCout = oldDate + instance.coutPieceMachine[pieceActuelle][rangPiece - 1];
            ancientCout = solution.dates[pieceActuelle][rangPiece];

            if (nouveauCout > ancientCout) {
                solution.dates[pieceActuelle][rangPiece] = nouveauCout;
                solution.pere[pieceActuelle][rangPiece].rang = rangPiece - 1;
                solution.pere[pieceActuelle][rangPiece].piece = pieceActuelle;
            }
        }

        machineCourrante = instance.machines[pieceActuelle][rangPiece];

        // arc vertical (disjonctif)
        if (ordreSurMachine[machineCourrante][0] != 0) {
            numeroPieceMachine = ordreSurMachine[machineCourrante][0];
            rangPieceMachine = ordreSurMachine[machineCourrante][1];
            oldDate = solution.dates[numeroPieceMachine][rangPieceMachine];

            nouveauCout = oldDate + instance.coutPieceMachine[numeroPieceMachine][rangPieceMachine];
            ancientCout = solution.dates[pieceActuelle][rangPiece];

            if (nouveauCout > ancientCout) {
                solution.dates[pieceActuelle][rangPiece] = nouveauCout;
                solution.pere[pieceActuelle][rangPiece].rang = rangPieceMachine;
                solution.pere[pieceActuelle][rangPiece].piece = numeroPieceMachine;
            }
        }

        // maj de l'ordre des pieces retenu sur la machine actuelle
        ordreSurMachine[machineCourrante][0] = pieceActuelle;
        ordreSurMachine[machineCourrante][1] = rangPiece;

        // maj du cout de la solution quand on arrive à la dernirère machine
        if (rangPiece == instance.nbMachines) {
            if (solution.dates[pieceActuelle][rangPiece] + instance.coutPieceMachine[pieceActuelle][rangPiece] > solution.count) {
                solution.count = solution.dates[pieceActuelle][rangPiece] + instance.coutPieceMachine[pieceActuelle][rangPiece];
                solution.pereEtoile.piece = pieceActuelle;
                solution.pereEtoile.rang = rangPiece;
            }
        }
    }
    solution.h = hachage(solution, instance);
}

void recherchePosition(t_solution& solution, t_instance& instance, int pieceCour, int rangCour, int rangPred, int piecePred, int& indexCour, int& indexPred) {
  bool predTrouve = false;
  int rangICour = instance.nbMachines;
  int rangIPred = instance.nbMachines;

  for (int i = instance.nbMachines * instance.nbPieces; !predTrouve && i >= 1; --i) {
    if (solution.bierwith[i] == pieceCour) {
      if (rangICour == rangCour)
        indexCour = i;
      else
        rangICour -= 1;
    }
    if (solution.bierwith[i] == piecePred) {
      if (rangIPred == rangPred) {
        indexPred= i;
        predTrouve = true;
      } else {
        rangIPred -= 1;
      }
    }
  }
}

void permuter(t_solution& solution, int i, int j) {
  int c = solution.bierwith[i];
  solution.bierwith[i] = solution.bierwith[j];
  solution.bierwith[j] = c;
}

void rechercheLocale(t_solution& solution, t_instance& instance, int maxIter) {
    int iter = 1;
    int stop = 0;
    int rangCour, pieceCour, rangPred, piecePred;
    int indexCour, indexPred;
    t_solution solutionPrime;
    int machineCour, machinePred;
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
    while ((iter < maxIter) && (stop == 0))
    {
      // test même machine
      if (machineCour == machinePred) {
        solutionPrime = solution;
        // modification de `solutionPrime`
        recherchePosition(solution, instance, pieceCour, rangCour, rangPred, piecePred, indexCour, indexPred);
        permuter(solutionPrime, indexCour, indexPred);
        // evaluation de la nouvelle solution
        evaluer(solutionPrime, instance);

        // si on améliore la solution
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

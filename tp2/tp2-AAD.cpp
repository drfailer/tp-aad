/* TODO:
SDD:
- t_instance (n, m, pij, mij)
- t_solution (cout, tij -> dates, pere, vecteur_bierwith)

FONCTION:
- lireFichier() -> instance
- genereVecteur(solution) -> vecteur_bierwith
- evaluer(s) -> solution
- rechLocale(s) -> ds
*/

#include "utils.h"
#include "tp2-AAD.h"
#include <iostream>
#include <stdlib.h>

int main() {
  t_solution solution = {0};
  t_instance instance = {0};
  srand(0);

  lectureFichier(instance, "./test/la01.txt");
  grasp(solution, instance, 10, 10, 1000);
  std::cout << "solution: " << solution.count << std::endl;

  lectureFichier(instance, "./test/la02.txt");
  grasp(solution, instance, 10, 10, 1000);
  std::cout << "solution: " << solution.count << std::endl;

  /* lectureFichier(instance, "./test/la_test.txt"); */
  /* grasp(solution, instance, 2, 2, 1000); */
  /* std::cout << "solution: " << solution.count << std::endl; */
}

void premierTest(t_solution &solution, t_instance &instance) {
  t_solution best;
  best.count = inf;
  for (int i = 0; i < 2000000; ++i) {
    genererVecteurBierwith(solution, instance);
    evaluer(solution, instance);
    if (best.count > solution.count)
      best = solution;
  }
  std::cout << best.count << std::endl;
}

void afficher_solution(t_instance &instance, t_solution &solution) {
  for (int i = 1; i <= instance.nbPieces; ++i) {
    for (int j = 1; j <= instance.nbMachines; ++j) {
      std::cout << solution.pere[i][j].piece << " " << solution.pere[i][j].rang
                << "  ";
    }
    std::cout << std::endl;
  }

  std::cout << solution.pereEtoile.piece << " " << solution.pereEtoile.rang
            << std::endl;

  for (int i = 1; i <= instance.nbPieces; ++i) {
    for (int j = 1; j <= instance.nbMachines; ++j) {
      std::cout << solution.dates[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << solution.count << std::endl;
}

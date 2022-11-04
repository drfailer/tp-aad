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

#include "tp2-AAD.h"
#include "utils.h"
#include <iostream>
#include <stdlib.h>

std::string jeuDeTest[11] = {"_test", "01", "02", "03", "04", "05",
                             "06",    "07", "11", "16", "20"};

int main() {
  t_solution solution = {0};
  t_instance instance = {0};
  srand(0);

  for (int i = 1; i < 11; ++i) {
    lectureFichier(instance, "./test/la" + jeuDeTest[i] + ".txt");
    std::cout << "./test/la" + jeuDeTest[i] + ".txt" << std::endl;
    grasp(solution, instance, 10, 10, 1000);
    std::cout << "solution: " << solution.count << std::endl;
  }
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

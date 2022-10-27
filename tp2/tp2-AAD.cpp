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

#include <iostream>
#include <ostream>
#include <stdlib.h>
#include "utils.h"
#include "tp2-AAD.h"

int main()
{
    t_solution solution = { 0 };
    t_instance instance = { 0 };
    srand(0);
    lectureFichier(instance, "./test/la01.txt");

    genererVecteurBierwith(solution, instance);
    /* evaluer(solution, instance); */
    rechercheLocale(solution, instance, 1000000000);
    std::cout << solution.count << std::endl;

    // afficher_solution(instance, solution);
    /* premierTest(solution, instance); */
}

void premierTest(t_solution& solution, t_instance& instance)
{
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

void afficher_solution(t_instance& instance, t_solution& solution)
{
    for (int i = 1; i <= instance.nbPieces; ++i) {
        for (int j = 1; j <= instance.nbMachines; ++j) {
            std::cout << solution.pere[i][j].piece << " " << solution.pere[i][j].rang << "  ";
        }
        std::cout << std::endl;
    }

    for (int i = 1; i <= instance.nbPieces; ++i) {
        for (int j = 1; j <= instance.nbMachines; ++j) {
            std::cout << solution.dates[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << solution.count << std::endl;
}

// tp-AAD.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "graphe.h"
#include "tp-AAD.h"

void afficher_solution(solution_t solution, int n, int nfin) {
    /* affichage */
    for (int i = 1; i <= n; ++i) {
        std::cout << i << " = " << solution.pere[i] << std::endl;
    }

    std::cout << std::endl << "date max:" << std::endl;
    std::cout << solution.count << std::endl << std::endl;

    std::cout << "dates:" << std::endl;
    for (int i = 1; i <= n; ++i) {
        std::cout << solution.date[i] << std::endl;
    }
}

void afficher_graphe(graphe_t& graphe)
{
    std::cout << graphe.n << std::endl;
    for (int i = 1; i <= graphe.n; ++i) {
        for (int j = 1; j <= graphe.ns[i]; ++j) {
            std::cout << "graphe.s[" << i << "][" << j << "] = " << graphe.s[i][j] << std::endl;
            std::cout << "graphe.d[" << i << "][" << j << "] = " << graphe.d[i][j] << std::endl;
            std::cout << "graphe.l[" << i << "][" << j << "] = " << graphe.l[i][j] << std::endl;
        }
    }
}

int main()
{
    solution_t solution = { 0 };
    graphe_t graphe = { 0 };
    int ordre[nmax] = { 0 };

    //graphe_fichier("graphe1.txt", graphe);
    graphe_fichier2("graphe2.txt", graphe);

    //afficher_graphe(graphe);

    /* test de l'algo de dijkstra (graphe2) */
    /*dijkstra(graphe, solution, 1, 210);
    afficher_solution(solution, graphe.n, 210);
    std::cout << std::endl;
    afficher_plus_court_chemin(solution, 210);*/

    /* Bellman */
    for (int i = 1; i <= graphe.n; ++i) {
        ordre[i] = i;
    }
    bellman(graphe, solution, ordre, 1, 210);
    std::cout << std::endl;
    afficher_plus_court_chemin(solution, 210);

    for (int i = 2; i <= graphe.n; ++i) {
        ordre[i] = 211 - i;
    }
    ordre[210] = 210;
    bellman(graphe, solution, ordre, 1, 210);
    std::cout << std::endl;
    afficher_plus_court_chemin(solution, 210);
}

#pragma once
#include <string>
#include <fstream>
#include <iostream>

const int nmax = 500;
const int nmax_succ = 50;
const int inf = 99999;

typedef struct {
	int n; // nb de noeud
	int ns[nmax]; // nb de successeurs par noeuds
	int s[nmax][nmax_succ]; // successeurs par noeuds
	float d[nmax][nmax_succ]; // cout des arcs
	float l[nmax][nmax_succ];
} graphe_t;

typedef struct {
	int pere[nmax];
	float date[nmax];
	float count; // date max
} solution_t;


/* FONCTIONS: */

void dijkstra(graphe_t graphe, solution_t& solution, int ndeb, int nfin);
void graphe_fichier(std::string name, graphe_t& graphe);
void graphe_fichier2(std::string name, graphe_t& graphe);
void afficher_plus_court_chemin(solution_t solution, int nfin);
void bellman(graphe_t graphe, solution_t& solution, int ordre[nmax], int ndeb, int nfin);
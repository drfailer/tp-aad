#include "graphe.h"

/* Lecture fichier:
* nb_noeud
* noeud nb_succ succ cout_arc ...
*/
void graphe_fichier(std::string name, graphe_t& graphe) {
	std::ifstream fichier(name, std::ios::in);
	int noeud_courrant;

	if (fichier) {
		fichier >> graphe.n; // recuperation du nombre de noeuds

		for (int i = 0; i < graphe.n; ++i) { // recuperation des noeuds et succ
			fichier >> noeud_courrant;
			fichier >> graphe.ns[noeud_courrant]; // nombre de voisins
			for (int j = 1; j <= graphe.ns[noeud_courrant]; ++j)
			{ // recuperation des voisins
				fichier >> graphe.s[noeud_courrant][j] >> graphe.d[noeud_courrant][j];
			}
		}
	}
	else {
		std::cerr << "Impossible d'ouvrir le fichier!" << std::endl;
	}
}

/*
* Recuper graphe dans un fichier de la forme:
* from to long duree
*/
void graphe_fichier2(std::string name, graphe_t& graphe) {
	std::ifstream fichier(name, std::ios::in);
	int from, to;
	int lines;

	if (fichier) {
		fichier >> graphe.n >> lines; // recuperation du nombre de noeuds

		for (int i = 0; i < lines; ++i) { // recuperation des noeuds et succ
			fichier >> from >> to;
			graphe.ns[from]++; // nombre de voisins
			graphe.s[from][graphe.ns[from]] = to;
			fichier >> graphe.l[from][graphe.ns[from]] >> graphe.d[from][graphe.ns[from]];
		}
	}
	else {
		std::cerr << "Impossible d'ouvrir le fichier!" << std::endl;
	}
}

/* recherche l'index du noeud non traite portant la marque minimale. */
int rech_min(float m[nmax], int T[nmax], int n) {
	int index_min = 1;
	int valmin = inf;

	for (int j = 2; j <= n; ++j) {
		if (m[j] < valmin && T[j] == 0) {
			index_min = j;
			valmin = m[j];
		}
	}
	return index_min;
}

/* Initialisation des tableaux de l'algo de dijkstra */
void init_dijkstra(float m[nmax], int T[nmax], solution_t& solution, int n, int ndeb) {
	for (int i = 1; i <= n; i++)
	{
		m[i] = inf;
		T[i] = 0;
		solution.pere[i] = -1;
		solution.date[i] = 0;
	}
	m[ndeb] = 0;
}

void dijkstra(graphe_t graphe, solution_t& solution, int ndeb, int nfin) {
	float m[nmax];
	int T[nmax];
	int noeud_courrant;
	float cout_arc;
	int index_succ;

	// initialisation
	init_dijkstra(m, T, solution, graphe.n, ndeb);

	for (int i = 1; i <= graphe.n; i++)
	{
		/* Recherche du min */
		noeud_courrant = rech_min(m, T, graphe.n);

		/* Mise à jour des successeurs */
		for (int succ = 1; succ <= graphe.ns[noeud_courrant]; ++succ) {
			index_succ = graphe.s[noeud_courrant][succ];
			cout_arc = graphe.d[noeud_courrant][succ];

			/* maj de la marque et de la solution si on trouve une date plus faible */
			if (m[noeud_courrant] + cout_arc < m[index_succ]) {
				m[index_succ] = m[noeud_courrant] + graphe.d[noeud_courrant][succ];
				solution.pere[index_succ] = noeud_courrant;
				solution.date[index_succ] = solution.date[noeud_courrant] + cout_arc;
			}
		}
		T[noeud_courrant] = 1;
	}
	solution.count = m[nfin];
}

void afficher_plus_court_chemin(solution_t solution, int nfin) {
	int i = nfin;

	std::cout << "Plus court chemin:" << std::endl << nfin << " ";
	while (i != -1) {
		std::cout << solution.pere[i] << " ";
		i = solution.pere[i];
		solution.date[i] = 0;
	}
	std::cout << std::endl;
}

void bellman(graphe_t graphe, solution_t& solution, int ordre[nmax], int ndeb, int nfin) {
	bool continuer = true;
	int noeud_courrant;
	int index_succ;
	float duree_arc;
	float m[nmax];

	/* Initialisation */
	for (int i = 1; i <= graphe.n; ++i) {
		m[i] = inf;
		solution.pere[i] = -1;
	}
	m[ndeb] = 0;

	int compteur = 0;

	while (continuer) {
		continuer = false;

		compteur++;

		if (compteur > 210)
		{
			std::cout << compteur << std::endl;;

			std::cout << " T : ";
			for (int j = 1; j <= 210; j++)
			{
				std::cout << solution.date[j] << "  ";
				if (j % 20 == 0)
					std::cout << std::endl;
			}

			std::cout << "------" << std::endl;
		}


		for (int i = 1; i <= graphe.n; ++i) {
			noeud_courrant = ordre[i];

			for (int succ = 1; succ <= graphe.ns[noeud_courrant]; ++succ) {
				index_succ = graphe.s[noeud_courrant][succ];
				duree_arc = graphe.d[noeud_courrant][succ];

				/* maj de la marque et de la solution si on trouve une date plus faible */
				if (m[noeud_courrant] + duree_arc < m[index_succ]) {
					m[index_succ] = m[noeud_courrant] + duree_arc;
					solution.pere[index_succ] = noeud_courrant;
					solution.date[index_succ] = solution.date[noeud_courrant] + duree_arc;
					continuer = true;
				}
			}
		}
	}
	solution.count = solution.date[nfin];
}

void graphe_par_niveau(graphe_t graphe, solution_t& solution) {
	/* TODO */
}
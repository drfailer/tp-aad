#pragma once
#include <iostream>

/* CONSTANTES: */

const int nmax = 50; // ATTENTION: ne pas mettre une valeur supérieur à 80 sous windows
const int k = 99999;
const int inf = 99999;

/* STRUCTURES: */

typedef struct {
	int nbMachines;
	int nbPieces;
	int machines[nmax][nmax];
	int coutPieceMachine[nmax][nmax];
} t_instance;

typedef struct {
	int rang;
	int piece;
} t_pere;

typedef struct {
	int count;
	int dates[nmax][nmax];
	t_pere pere[nmax][nmax];
	int bierwith[nmax * nmax];
	int h;
	t_pere pereEtoile;
} t_solution;

/* FONCTIONS: */

void lectureFichier(t_instance& instance, std::string name);
void genererVecteurBierwith(t_solution& solution, t_instance& instance);
void evaluer(t_solution& solution, t_instance& instance);
void rechercheLocale(t_solution& solution, t_instance& instance, int maxIter);
void grasp(t_solution& solution, t_instance& instance, int nbELS, int nbVoisin, int nbIter);
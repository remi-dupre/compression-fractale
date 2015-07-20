/*
 * Contient es structures nécéssaires à la représentation d'une IFS
 */

struct LinReg;
struct Transformation;
struct Correspondance;
struct IFS;

#ifndef FORMAT
#define FORMAT

#include <iostream>
#include <vector>
#include "ImageMatricielle.h"
#include "ImageFractale.h"

/* *************** Définition de structures *************** */

typedef struct LinReg LinReg;
struct LinReg {
	/* Représente une fonction affine
	 * f(x) = a*x + b
	*/
	float a, b;
};

typedef struct Transformation Transformation;
struct Transformation {
	/* Décrit une transformation affine appliquée à un bloc
	 *  - translation :
	 *  - rotation : en degrés
	 *  - decalage : le décalage de couleur à ajouter
	 */
	unsigned short int rotation;		// Rotation par rapport au centre de la parcelle en ° (0 à 360)
	LinReg droite;		 	// La correction à appliquer à la couleur
};
#define ROTATION(rot) {rot, {1,0}} // Initialisation d'une transformation de type rotation

typedef struct Correspondance Correspondance;
struct Correspondance {
	/* Décrit un couple bloc/transformation */
	int spliter; // Le nombre de fois qu'il faut découper le bloc avant d'y appliquer la transformation
	int bloc; // l'indice du bloc à choisir dans l'image
	Transformation transformation; // le type de transformation à y appliquer
};

typedef struct IFS IFS;
struct IFS {
	/* Décrit tout ce qu'il faut savoir sur un ifs */
	int decoupePetit; // Taille de découpe pour les images
	int decoupeGros;  // Taille de découpe pour les antécédants
	std::vector<Correspondance> correspondances; // L'ifs : la liste de la transformation/antécédant de chaque bloc
};

/* *************** Fonctions *************** */

unsigned char couleurLinReg(const LinReg& droite, unsigned char couleur);

#endif

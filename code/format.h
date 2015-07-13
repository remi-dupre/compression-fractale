struct LinReg;
struct Transformation;
struct Correspondance;
struct IFS;

#ifndef FORMAT
#define FORMAT

#include <iostream>
#include <vector>
#include "ImageMatricielle.h"

/* *************** Définition de structures *************** */

typedef struct Coordonnees Coordonnees;
struct Coordonnes {
	/* Des coordonnées entières dans deux dimensions */
	int x;
	int y;
};

typedef struct LinReg LinReg;
struct LinReg {
	/* Représente une fonction affine
	 * f(x) = a*x + b
	*/
	double a, b;
};

typedef struct Transformation Transformation;
struct Transformation {
	/* Décrit une transformation affine appliquée à un bloc
	 *  - translation :
	 *  - rotation : en degrés
	 *  - decalage : le décalage de couleur à ajouter
	 */
	int rotation;           // Rotation par rapport au centre de la parcelle
	LinReg droite;          // La correction à appliquer à la couleur
};
#define ROTATION(rot) {rot, {1,0}} // Initialisation d'une transformation de type rotation

typedef struct Correspondance Correspondance;
struct Correspondance {
	/* Décrit un couple bloc/transformation */
	int bloc; // l'indice du bloc à choisir dans l'image
	Transformation transformation; // le type de transformation à y appliquer
};

typedef struct IFS IFS;
struct IFS {
	/* Décrit tout ce qu'il faut savoir sur un ifs */
	unsigned int decoupePetit; // Taille de découpe pour les images
	unsigned int decoupeGros;  // Taille de découpe pour les antécédants
	std::vector<Correspondance> correspondances; // L'ifs : la liste de la transformation/antécédant de chaque bloc
};

typedef struct ImageFractale ImageFractale;
struct ImageFractale {
	int largeur, hauteur;
	bool couleur, transparence;
	std::vector<int> moyenne;
	std::vector<IFS> ifs;
};

/* *************** Fonctions *************** */

int couleurLinReg(const LinReg& droite, int couleur);
void sauvegarder(const char* fichier, const ImageFractale& imageFractale);
ImageFractale compresser(const char* fichier, int precisionPetit, int precisionGros, bool couleur = false, bool transparence = false);

#endif

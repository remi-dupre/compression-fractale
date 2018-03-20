/*
 * Contient les structures nécéssaires à la représentation d'une IFS
 */
/*  Contains the structures needed to represent an IFS   */
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
    /* Represents an affine function
     * f (x) = a * x + b
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
    /* Describes an affine transformation applied to a block
      * - translation:
      * - rotation: in degrees
      * - offset: the color shift to add
   */
	unsigned short int rotation;		// Rotation par rapport au centre de la parcelle en ° (0 à 360)  // Rotation relative to the center of the plot in ° (0 to 360)
	LinReg droite;		 	// La correction à appliquer à la couleur   // The correction to apply to the color
};
#define ROTATION(rot) {rot, {1,0}} // Initialisation d'une transformation de type rotation  // Initializing a rotation transformation

typedef struct Correspondance Correspondance;
struct Correspondance {
	/* Décrit un couple bloc/transformation // Describes a couple block / transformation */
	int spliter; // Le nombre de fois qu'il faut découper le bloc avant d'y appliquer la transformation // The number of times to cut the block before applying the transformation
	int bloc; // l'indice du bloc à choisir dans l'image  // the index of the block to choose from the image
	Transformation transformation; // le type de transformation à y appliquer  //  the type of transformation to apply to it
};

typedef struct IFS IFS;
struct IFS {
	/* Décrit tout ce qu'il faut savoir sur un ifs // Describes everything you need to know about an ifs */
	int decoupePetit; // Taille de découpe pour les images // Cut size for images
	int decoupeGros;  // Taille de découpe pour les antécédants // Cutting size for antecedents
	std::vector<Correspondance> correspondances; // L'ifs : la liste de la transformation/antécédant de chaque bloc // The ifs: the list of the transformation / antecedent of each block
};

/* *************** Fonctions *************** */

unsigned char couleurLinReg(const LinReg& droite, unsigned char couleur);

#endif

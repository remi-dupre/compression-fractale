/*
 * Définition des structures et des fonctions utilisées pour enregistrer dans un fichier
 */

struct Flotant16b;
struct Pack_Entete;
struct Pack_IFS;
struct Pack_Correspondance;

#ifndef FORMAT_FICHIER
#define FORMAT_FICHIER

#include <cmath> // pow, frexp
#include "ImageFractale.h"

/* *************** Structures *************** */

#define DECALAGE_EXPOSANT 2 // Plus précis pour les petites valeurs
#define TAILLE_MANTISSE 1024

struct Flotant16b {
    /* Représente un flotant sur 16 bits */
	short signed int exp       : 5;
	short signed int mantisse  : 11; // log2(TAILLE_MANTISSE)

	Flotant16b() { mantisse = exp = 0; }	// Un constructeur élémentaire
    Flotant16b(float x) {					// Constructeur à partir d'un float 32 bits
    	int e;
    	mantisse = 1024 * std::frexp(x, &e);
    	exp = e - DECALAGE_EXPOSANT;
    }
};

float decode16bFloat(Flotant16b x);

struct Pack_Entete {
    /* L'en-tête du document
     *  sur 32 bits
     */
	unsigned int largeur   :15;
	unsigned int hauteur   :15;
	bool couleur           :1;
	bool transparence      :1;
};

struct Pack_IFS {
    /* L'en-tête d'une couche
     *  sur 32 bits
     */
	unsigned int decoupeGros	:12;
	unsigned int decoupePetit	:12;
	unsigned char moyenne		:8;
};

#define MAX_GROS_BLOCS 4095
#define SIZEOF_PACK_CORRESPONDANCE 6

struct Pack_Correspondance {
    /* Une correspondance
     *  sur 32 bits
     *  avec a (float) : 48 bits
     */
	unsigned int bloc		:12;	// Limité à 4095 gros blocs
	unsigned int rotation	:8;		// 0 à 255, proportionel à l'angle
	signed int b			:12;	// -512 à 512
	Flotant16b a;		//	:16;	// Flotant 16 bits
	// : 16 bits de vide 
};

/* *************** Fonctions de mise en paquets *************** */

#define WARNING_PACKING true // Doit t'on afficher un message dans la console si une entrée semble dangereuse

Pack_Entete packer_entete(const ImageFractale&);
Pack_IFS packer_ifs(const IFS& ifs, unsigned char moyenne = 0);
Pack_Correspondance packer_correspondance(const Correspondance&);

unsigned char unpack_moyenne(const Pack_IFS&);
IFS unpack_IFS(const Pack_IFS&);
Correspondance unpack_correspondance(const Pack_Correspondance&);

#endif

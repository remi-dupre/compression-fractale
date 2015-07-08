#include "ImageMatricielle.h"

/* *************** Constructeur / Destructeur *************** */

ImageMatricielle::ImageMatricielle(int x, int y) : largeur(x), hauteur(y) {
	/* Créée une nouvelle image de dimensions données
	 * Les pixels de l'image ne sont pas initialisés
	 */
	image = new int* [largeur];
	for(int i=0 ; i<largeur ; i++) {
		image[i] = new int[hauteur];
	}
}

ImageMatricielle::~ImageMatricielle() {
	/* Suppression de l'image */
	for(int i=0 ; i<largeur ; i++) {
		delete[] image[i];
	}
	delete[] image;
}

/* *************** Setters / Getters *************** */

int* ImageMatricielle::operator[](int i) {
	/* Retourne la ligne de l'image correspondante */
	return image[i];
}

int ImageMatricielle::getHauteur() const { return hauteur; }
int ImageMatricielle::getLargeur() const { return largeur; }

/* *************** Découpe *************** */

std::vector<ImagePart>* ImageMatricielle::decouper(int taille) {
	/* Découpe l'image en sous-images
	 * Les sous parties sont des carrés de côté "taille", le dépassement est ignoré
	 */
	std::vector<ImagePart>* liste = new std::vector<ImagePart>();
	int nbLargeur = largeur/taille;
	int nbHauteur = hauteur/taille;
	for(int i=0 ; i<nbLargeur ; i++) {
		for(int j=0 ; j<nbHauteur ; j++) {
			liste->push_back( ImagePart(this, i*taille, j*taille, taille) );
		}
	}
	return liste;
}


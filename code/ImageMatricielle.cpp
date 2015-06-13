#include "ImageMatricielle.h"

ImageMatricielle::ImageMatricielle(int x, int y) : largeur(x), hauteur(y) {
	image = new int* [largeur];
	for(int i=0 ; i<largeur ; i++) {
		image[i] = new int[y];
	}
}

ImageMatricielle::~ImageMatricielle() {
	/* Suppression de l'image */
	for(int i=0 ; i<largeur ; i++) {
		delete[] image[i];
	}
	delete[] image;
}

int* ImageMatricielle::operator[](int i) {
	/* Retourne la bonne ligne de l'image */
	return image[i];
}

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

int ImageMatricielle::getHauteur() {
	return hauteur;
}

int ImageMatricielle::getLargeur() {
	return largeur;
}

void ImageMatricielle::setHauteur(int _hauteur) {
	hauteur = _hauteur;
}

void ImageMatricielle::setLargeur(int _largeur) {
	largeur = _largeur;
}

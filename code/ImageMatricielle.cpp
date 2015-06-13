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

#include "ImagePart.h"

ImagePart::ImagePart(ImageMatricielle* maman, int _x, int _y, int _taille) :
	source(maman), taille(_taille), x(_x), y(_y) {
}

void ImagePart::set(int i, int j, int val) {
	(*source)[i+x][j+y] = val;
}

void ImagePart::remplir(int couleur) {
	/* Remplis le bout d'image avec une couleur uniforme */
	for(int i=0 ; i<taille ; i++) {
		for(int j=0 ; j<taille ; j++) {
			set(i, j, couleur);
		}
	}
}

int ImagePart::at(int i, int j) {
	return (*source)[i+x][j+y];
}

int ImagePart::couleurMoyenne() {
	/* La moyenne des couleurs représentées sur l'image */
	int somme = 0;
	for(int i=0 ; i<taille ; i++) {
		for(int j=0 ; j<taille ; j++) {
			somme += at(i, j);
		}
	}
	return somme/(taille*taille);
}

float ImagePart::varianceDifference(ImagePart B) {
	/* Compare deux images :
	 * Etudie la variance des "distances" entre les pixels
	 * La moyenne de chaque image est ajustée par ajout d'une constante
	 */
	int ecart = B.couleurMoyenne() - couleurMoyenne();
	int somme = 0;
	for(int i=0 ; i<taille ; i++) {
		for(int j=0 ; j<taille ; j++) {
			somme += std::pow(std::abs( at(i, j) - B.at(i,j) + ecart ), 2);
		}
	}
	return float(somme)/float(taille*taille);
}

void ImagePart::debug() {
	for(int i=0 ; i<taille ; i++) {
		for(int j=0 ; j<taille ; j++) {
			std::cout << "" << at(i, j) << " ";
		}
		std::cout << "\n";
	}
}

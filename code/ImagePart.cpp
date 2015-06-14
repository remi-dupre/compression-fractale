#include "ImagePart.h"

ImagePart::ImagePart(ImageMatricielle* maman, int _x, int _y, int _taille) :
	source(maman), taille(_taille), x(_x), y(_y) {
	virtuel = false;
}

ImagePart::ImagePart(int _taille) : taille(_taille) {
	/* Crée un "faux" bout d'image */
	virtuel = true;
	x = y = 0;
	source = new ImageMatricielle(taille, taille);
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

void ImagePart::transformer(ImagePart& imgSortie, Transformation transfo) {
	/* Applique une transformation linéaire sur la partie d'image
	 * La transformation est appliqué sur la partie passée en argument
	 */
	int a = imgSortie.getTaille();
	std::complex<double> rapport = std::polar(transfo.rapport, rad(transfo.rotation));
	std::complex<double> centre(transfo.translation_x+taille/2, transfo.translation_y+taille/2);
	for(int is=0 ; is<a ; is++) {
		for(int js=0 ; js<a ; js++) {
			std::complex<double> origine(is, js);
			std::complex<double> sortie = (rapport * (origine - centre)) + centre;
			int i = rint(sortie.real());
			int j = rint(sortie.imag());
			//std::cout << "["<<i<<","<<j<<"] -> ["<<is<<","<<js<<"]\n";
			//std::cout << origine << " -> " << sortie << " -> ["<<i<<","<<j<<"]\n";
			imgSortie.set(is, js, at(i, j));
		}
	}
}

int ImagePart::at(int i, int j) {
	return (*source)[i+x][j+y];
}

int ImagePart::getTaille() { return taille; }

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
			std::cout << "" << at(j, i) << " ";
		}
		std::cout << "\n";
	}
}

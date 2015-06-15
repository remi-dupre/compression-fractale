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

ImagePart::~ImagePart() {
	if(virtuel) { // L'image source a été créée par l'objet et doit être supprimée
		delete source;
	}
}

void ImagePart::set(int i, int j, int val) {
	/* Modifie la valeur d'un élément dans la parties
	 *  /!\ Interdit les modifications de l'extérieur
	 */
	if( i>=0 && j>=0 && i<taille && j<taille) {
		(*source)[i+x][j+y] = val;
	}
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
	double rapportx = (double(taille)/a)*cos(rad(transfo.rotation)); // r*e^(i*theta)
	double rapporty = (double(taille)/a)*sin(rad(transfo.rotation));
	double centrex = transfo.translation.x + (taille/2); // Centre de la rotation
	double centrey = transfo.translation.y + (taille/2);
	
	for(int is=0 ; is<a ; is++) {
		for(int js=0 ; js<a ; js++) {
			int i = rint( (rapportx*(is-centrex)) - (rapporty*(js-centrey)) + centrex );
			int j = rint( (rapporty*(js-centrey)) + (rapportx*(is-centrex)) + centrey );
			imgSortie.set(is, js, at(i, j));
		}
	}
}

int ImagePart::at(int i, int j) {
	int ix = i+x;
	int jy = j+y;
	
	if(ix < 0) i = 0;
	else if(ix >= source->getLargeur()) ix = source->getLargeur() - 1;
	if(jy < 0) j = 0;
	else if(jy >= source->getHauteur()) jy = source->getHauteur() - 1;
	
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

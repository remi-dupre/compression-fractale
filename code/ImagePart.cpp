#include "ImagePart.h"

/* *************** Constructeur / Destructeur *************** */

ImagePart::ImagePart(ImageMatricielle* maman, int _x, int _y, int _taille) :
	/* Pointe une partie d'image.
	 * Entrées :
	 *   - maman : un pointeur sur l'image source
	 *   - x,y : les coordonnées du point haut-gauche du carré
	 *   - taille : la cote du carrés
	 */
	source(maman), taille(_taille), x(_x), y(_y) {
	virtuel = false;
}

ImagePart::ImagePart(int _taille) : taille(_taille) {
	/* Crée un "faux" bout d'image.
	 * Entrées :
	 *   - taille : la cote du carré */
	virtuel = true;
	x = y = 0;
	source = new ImageMatricielle(taille, taille);
}

ImagePart::~ImagePart() {
	if(virtuel) { // L'image source a été créée dans le constructeur
		delete source;
	}
}

/* *************** Setteurs / Getteurs *************** */

void ImagePart::set(int i, int j, int val) {
	/* Modifie la valeur d'un élément dans la parties
	 * /!\ Interdit les modifications de l'extérieur
	 * Entrées :
	 *   - i,j : les coordonnées du pixel à modifier, dans [0, taille[
	 *   - val : la valeur à lui attribuer
	 */
	if( i>=0 && j>=0 && i<taille && j<taille ) {
		(*source)[i+x][j+y] = val;
	}
}

int ImagePart::at(int i, int j) const {
	/* Retourne la valeur aux coordonnées données (i,j)
	 * Si les coordonnées dépassent du blocs mais restent dans l'image ça marche quand même
	 */
	int ix = i+x;
	int jy = j+y;
	
	if(ix < 0) ix = 0;
	else if(ix >= source->getLargeur()) ix = source->getLargeur() - 1;
	
	if(jy < 0) jy = 0;
	else if(jy >= source->getHauteur()) jy = source->getHauteur() - 1;
	
	return (*source)[ix][jy];
}

int ImagePart::getTaille() const { return taille; } // La cote du carré

int ImagePart::couleurMoyenne() const {
	/* La moyenne des couleurs représentées sur le bout d'image */
	int somme = 0;
	for(int i=0 ; i<taille ; i++) {
		for(int j=0 ; j<taille ; j++) {
			somme += at(i, j);
		}
	}
	return somme/(taille*taille);
}

void ImagePart::remplir(int couleur) {
	/* Remplis le bout d'image avec une couleur uniforme */
	for(int i=0 ; i<taille ; i++) {
		for(int j=0 ; j<taille ; j++) {
			set(i, j, couleur);
		}
	}
}

/* *************** Transformations *************** */

void ImagePart::transformer(ImagePart& imgSortie, const Transformation& transfo) {
	/* Applique une transformation linéaire sur la partie d'image
	 * Entrées :
	 *   - imgSortie : le bout d'image qui reçois le résultat de la transformation
	 *   - transfo : un type de transformation
	 * /!\ Il vaut mieux accompagner cette fonction d'un brouillon de calculs
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

Transformation ImagePart::chercherTransformation(ImagePart& origine, float& varianceRetour) {
	/* Cherche la meilleur transformation de origine pour correspondre à cet objet
	 * Entrées :
	 *   - origine : l'image qui subis les transformations
	 *   - varianceRetour : un flotant
	 * Sorties :
	 *   - retourne la tranformation optimale
	 *   - modifie varianceRetour : la variance des couleurs de l'image de référence et de l'autre après transformation
	 */
	Transformation max, min, mid;
	max.translation.x = max.translation.y = min.translation.x = min.translation.y = mid.translation.x = mid.translation.y = 0;
	max.rotation = 355;
	min.rotation = 0;
	
	ImagePart img(taille);
	
	origine.transformer(img, max);
	float varmax = varianceDifference(img);
	
	origine.transformer(img, min);
	float varmin = varianceDifference(img);
	
	while(max.rotation - min.rotation > 5) {
		mid.rotation = (max.rotation+min.rotation)/2;
		origine.transformer(img, mid);
		float variance = varianceDifference(img);
		if(varmin < varmax) {
			max.rotation = mid.rotation;
			varmax = variance;
		}
		else {
			min.rotation = mid.rotation;
			varmin = variance;
		}
	}
	varianceRetour = varmin;
	return min;
}

Source ImagePart::chercherMeilleur(std::vector<ImagePart>& parties) {
	/* Cherche la meilleur image d'origine pour une transformation
	 * Entrée :
	 *   - parties : un tableau de bouts d'images
	 * Sortie : un type source :
	 *   - bloc : l'indice du bout d'image choisis dans "parties"
	 *   - transformation : la transformation optimale pour ce blocs
	 */
	int n = parties.size();
	float varianceMax, variance;
	Transformation transfo = chercherTransformation(parties[0], varianceMax);
	Transformation transfoMax = transfo;
	transfo.translation.x = transfo.translation.y = transfo.rotation = 0;
	int imax = 0;
	for(int i=1 ; i<n ; i++) {
		transfo = chercherTransformation(parties[i], variance);
		if(varianceMax < variance) {
			imax = i;
			transfoMax = transfo;
			varianceMax = variance;
		}
	}
	Source retour;
	retour.bloc = imax;
	retour.transformation = transfoMax;
	return retour;
}
float ImagePart::varianceDifference(const ImagePart& B) {
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
	/* Un debug moche à l'arrache de l'image */
	for(int i=0 ; i<taille ; i++) {
		for(int j=0 ; j<taille ; j++) {
			std::cout << "" << at(j, i) << " ";
		}
		std::cout << "\n";
	}
}

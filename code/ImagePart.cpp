#include "ImagePart.h"

/* *************** Constructeur / Destructeur *************** */

ImagePart::ImagePart(ImageMatricielle* maman, int _x, int _y, int _taille) :
	/* Pointe une partie d'image.
	 * Entrées :
	 *   - maman : un pointeur sur l'image source
	 *   - x,y : les coordonnées du point haut-gauche du carré
	 *   - taille : la cote du carrés
	 */
	mSource(maman), mTaille(_taille), mX(_x), mY(_y) {
	mVirtuel = false;
}

ImagePart::ImagePart(int _taille) : mTaille(_taille) {
	/* Crée un "faux" bout d'image.
	 * Entrées :
	 *   - taille : la cote du carré */
	mVirtuel = true;
	mX = mY = 0;
	mSource = new ImageMatricielle(mTaille, mTaille);
}

ImagePart::~ImagePart() {
	if(mVirtuel) { // L'image source a été créée dans le constructeur
		delete mSource;
	}
}

/* *************** Setters / Getters *************** */

void ImagePart::set(int i, int j, int val) {
	/* Modifie la valeur d'un élément dans la parties
	 * /!\ Interdit les modifications de l'extérieur
	 * Entrées :
	 *   - i,j : les coordonnées du pixel à modifier, dans [0, taille[
	 *   - val : la valeur à lui attribuer
	 */
	if( i>=0 && j>=0 && i<mTaille && j<mTaille && i+mX<mSource->getLargeur() && j+mY<mSource->getHauteur() ) {
		(*mSource)[i+mX][j+mY] = val;
	}
}

int ImagePart::at(int i, int j) const {
	/* Retourne la valeur aux coordonnées données (i,j)
	 * Si les coordonnées dépassent du blocs mais restent dans l'image ça marche quand même
	 */
	int ix = i+mX;
	int jy = j+mY;

	if(ix < 0) ix = 0;
	else if(ix >= mSource->getLargeur()) ix = mSource->getLargeur() - 1;

	if(jy < 0) jy = 0;
	else if(jy >= mSource->getHauteur()) jy = mSource->getHauteur() - 1;

	return (*mSource)[ix][jy];
}

int ImagePart::getTaille() const { return mTaille; } // La cote du carré

int ImagePart::couleurMoyenne() const {
	/* La moyenne des couleurs représentées sur le bout d'image */
	int somme = 0;
	for(int i=0 ; i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			somme += at(i, j);
		}
	}
	return somme/(mTaille*mTaille);
}

void ImagePart::remplir(int couleur) {
	/* Remplis le bout d'image avec une couleur uniforme */
	for(int i=0 ; i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			set(i, j, couleur);
		}
	}
}

/* *************** Transformations *************** */

void ImagePart::transformer(ImagePart& imgSortie, const Transformation& transfo) const {
	/* Applique une transformation linéaire sur la partie d'image
	 * Entrées :
	 *   - imgSortie : le bout d'image qui reçois le résultat de la transformation
	 *   - transfo : un type de transformation
	 * /!\ Il vaut mieux accompagner cette fonction d'un brouillon de calculs
	 */
	int a = imgSortie.getTaille();
	double rapportx = (double(mTaille)/a)*cos(RAD(transfo.rotation)); // r*e^(i*theta)
	double rapporty = (double(mTaille)/a)*sin(RAD(transfo.rotation));
	double centrex = transfo.translation.x + (mTaille/2); // Centre de la rotation
	double centrey = transfo.translation.y + (mTaille/2);
	for(int is=0 ; is<a ; is++) {
		for(int js=0 ; js<a ; js++) {
			int i = rint( (rapportx*(is-centrex)) - (rapporty*(js-centrey)) + centrex );
			int j = rint( (rapporty*(is-centrey)) + (rapportx*(js-centrex)) + centrey );
			imgSortie.set(is, js, at(i, j) + transfo.decalage); // On a trouvé le point correspondant, on rajoute le décalage de couleur
		}
	}
}

Transformation ImagePart::chercherTransformation(const ImagePart& origine, float& varianceRetour) const {
	/* Cherche la meilleur transformation de origine pour correspondre à cet objet
	 * Entrées :
	 *   - origine : l'image qui subis les transformations
	 *   - varianceRetour : un flotant
	 * Sorties :
	 *   - retourne la tranformation optimale
	 *   - modifie varianceRetour : la variance des couleurs de l'image de référence et de l'autre après transformation
	 */

	Transformation max = ROTATION(355);
	Transformation min = ROTATION(0);
	Transformation mid = ROTATION(0);

	ImagePart img(mTaille);

	origine.transformer(img, max);
	float varmax = varianceDifference(img);

	origine.transformer(img, min);
	float varmin = varianceDifference(img);

	while(max.rotation - min.rotation > 5) {
		mid.rotation = (max.rotation+min.rotation)/2;
		origine.transformer(img, mid);
		float variance = varianceDifference(img, &mid.decalage);
		if(varmin < varmax) {
			max = mid;
			varmax = variance;
		}
		else {
			min = mid;
			varmin = variance;
		}
	}
	varianceRetour = varmin;
	return min;
}

Source ImagePart::chercherMeilleur(const std::vector<ImagePart>& parties) const {
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

float ImagePart::varianceDifference(const ImagePart& B, int *decalage) const {
	/* Compare deux images :
	 * Etudie la variance des "distances" entre les pixels
	 * La moyenne de chaque image est ajustée par ajout d'une constante
	 * Entrées :
	 *  - B : l'image a laquelle this est comparé
	 *  - decalage : un pointeur sur une variable qui prendra la valeur du décalage (peut être NULL)
	 *     -> decalage doit être ajouté aux pixels de B pour équilibrer la moyenne
	 */
	int ecart = couleurMoyenne() - B.couleurMoyenne();
	int somme = 0;
	for(int i=0 ; i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			somme += std::pow(std::abs( B.at(i,j) - at(i, j) + ecart ), 2);
		}
	}
	if(decalage != NULL) *decalage = ecart;
	return float(somme)/float(mTaille*mTaille);
}

void ImagePart::debug() const {
	/* Un debug moche à l'arrache de l'image */
	for(int i=0 ; i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			std::cout << "" << at(j, i) << " ";
		}
		std::cout << "\n";
	}
}

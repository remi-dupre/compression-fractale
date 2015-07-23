#include "ImagePart.h"

/* *************** Constructeur / Destructeur *************** */

ImagePart::ImagePart(ImageMatricielle* maman, int x, int y, int taille) :
	mImage(maman), mTaille(taille), mX(x), mY(y) {
	/* Pointe une partie d'image.
	 * Entrées :
	 *   - maman : un pointeur sur l'image source
	 *   - x,y : les coordonnées du point haut-gauche du carré
	 *   - taille : la cote du carré
	 */
	mVirtuel = false;
}

ImagePart::ImagePart(int taille) : mTaille(taille) {
	/* Crée un "faux" bout d'image.
	 * Entrées :
	 *   - taille : la cote du carré */
	mVirtuel = true;
	mX = mY = 0;
	mImage = new ImageMatricielle(mTaille, mTaille);
}

ImagePart::~ImagePart() {
	if(mVirtuel) { // L'image source a été créée dans le constructeur
		delete mImage;
	}
}

/* *************** Setters / Getters *************** */

void ImagePart::set(int i, int j, unsigned char val) {
	/* Modifie la valeur d'un élément dans la parties
	 * /!\ Interdit les modifications de l'extérieur
	 * Entrées :
	 *   - i,j : les coordonnées du pixel à modifier, dans [0, taille[
	 *   - val : la valeur à lui attribuer
	 */
	if( ( i>=0 && i<mTaille ) &&  ( j>=0 && j<mTaille ) ) // Ca ne dépasse pas de la parcelle
	if( i+mX < mImage->getLargeur() && j+mY < mImage->getHauteur() ) // ca ne dépasse pas de l'image
		(*mImage)[i+mX][j+mY] = val;
}

unsigned char ImagePart::at(int i, int j) const {
	/* Retourne la valeur aux coordonnées données (i,j)
	 * Si les coordonnées dépassent du blocs mais restent dans l'image ça marche quand même
	 */
	int ix = i+mX;
	int jy = j+mY;

	if(ix < 0) ix = 0; // Si ca dépasse on se projete sur le bord
	else if(ix >= mImage->getLargeur()) ix = mImage->getLargeur() - 1;

	if(jy < 0) jy = 0; // Si ca dépasse on se projete sur le bord
	else if(jy >= mImage->getHauteur()) jy = mImage->getHauteur() - 1;

	return (*mImage)[ix][jy];
}

int ImagePart::getTaille() const { return mTaille; } // La cote du carré

unsigned char ImagePart::couleurMoyenne() const {
	/* La moyenne des couleurs représentées sur le bout d'image */
	unsigned int somme = 0;
	for(int i=0 ; i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			somme += at(i, j);
		}
	}
	return somme/(mTaille*mTaille);
}

void ImagePart::remplir(unsigned char couleur) {
	/* Remplis le bout d'image avec une couleur uniforme */
	for(int i=0 ; i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			set(i, j, couleur);
		}
	}
}

/* *************** Régression linéaire *************** */

LinReg ImagePart::chercherLinReg(const ImagePart& X) const {
	/* On cherche une fonction affine f telle que f(X) ~= Y
	 * Les formules développées viennent de wikipédia et sont démontrable avec le programme de supp
	 */
	const ImagePart &Y = *this;
	double sumX, sumY, sumXY, sumXX; // On a besoins de calculer 4 grosses sommes
	sumX = sumY = sumXY = sumXX = 1; // On évite les divisions par 0
	for(int i=0 ;  i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			sumX += X.at(i, j);
			sumY += Y.at(i, j);
			sumXY += X.at(i, j) * Y.at(i, j);
			sumXX += std::pow(X.at(i, j), 2);
		}
	}
	double n = mTaille*mTaille;
	LinReg retour;
		retour.a = ( (sumX*sumY/n) - sumXY ) / ( (sumX*sumX/n) - sumXX );
		retour.a = decode16bFloat( Flotant16b( retour.a ) );
		retour.b = ( sumY - (retour.a*sumX) ) / n;
	return retour;
}

void ImagePart::appliquerLinReg(const LinReg& droite) {
	/* Applique la fonction affine droite à tous les pixels de la parcelle */
	for(int i=0 ; i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			set(i, j, couleurLinReg(droite, at(i, j)));
		}
	}
}

/* *************** Transformations *************** */

void ImagePart::transformer(ImagePart& imgSortie, const Transformation& transfo) const {
	/* Applique une transformation linéaire sur la partie d'image
	 * Entrées :
	 *   - imgSortie : le bout d'image dans lequel on enregistre le résultat de la transformation
	 *   - transfo : un type de transformation
	 * /!\ Il vaut mieux accompagner cette fonction d'un brouillon de calculs
	 */
	int a = imgSortie.getTaille();
	double rapportx = (double(mTaille)/a)*cos(RAD(transfo.rotation)); // r*e^(i*theta)
	double rapporty = (double(mTaille)/a)*sin(RAD(transfo.rotation));
	double centre = mTaille / 2; // Centre de la rotation, en x et en y
	for(int is=0 ; is<a ; is++) {
		for(int js=0 ; js<a ; js++) {
			int i = rint( (rapportx*(is-centre)) - (rapporty*(js-centre)) + centre ); // Calculs des parties imaginaires et réelles
			int j = rint( (rapporty*(is-centre)) + (rapportx*(js-centre)) + centre );
			imgSortie.set(is, js, couleurLinReg(transfo.droite, at(i, j))); // On a trouvé le point correspondant, on rajoute le décalage de couleur
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
	extern float SEUIL_LISSAGE, SEUIL_VARIANCE;

	Transformation max = ROTATION(360); // max sert juste de borne mais ne peut pas être la valeur de retour
	Transformation mid = ROTATION(0);
	Transformation min = ROTATION(0);
		min.droite.a = 0;					// Vérifie la droite verticale
		min.droite.b = couleurMoyenne();	// Comme ca on a la bonen couleur sur les bouts lisses

	ImagePart img(mTaille);

	origine.transformer(img, max);
	float varmax = varianceDifference(img); // max sert juste de borne

	origine.transformer(img, min);
	float varmin = varianceDifference(img, &min.droite, false); // Il faut donner une valeur à min.droite au cas où il est retourné
	//std::cout << min.droite.b << "-" << varmin << " ";

	/* Application de la dichotomie :
	 * La variance en fonction de l'angle n'est pas (/rarement) monotone, l'algorithme tend vers un minimum local
	 * A chaque itération on choisis l'angle moitié, puis mon garde l'angle de variance la plus faible et ce nouvel angle
	 *  -> min < max gardent leur ordre mais le qualificatif n'est relatif qu'à leur angle
	 *  -> de toutes facons, à la fin varmin ~= varmax
	 */
	LinReg droite;
	if(varmin > SEUIL_LISSAGE) {
		while(max.rotation - min.rotation > 5 && varmin > SEUIL_VARIANCE) {
			mid.rotation = (max.rotation + min.rotation) / 2; // On prend le milieu et on calcul la transformation
			origine.transformer(img, mid);
			float variance = varianceDifference(img, &droite);
			if(varmin < varmax) {
				max.rotation = mid.rotation;
				max.droite = droite;
				varmax = variance;
			}
			else {
				min.rotation = mid.rotation;
				min.droite = droite;
				varmin = variance;
			}
		}
	}
	varianceRetour = varmin; // On retourne la variance obtenue pour éviter de refaire le calcul
	return min;
}

bool ImagePart::chercherMeilleur(const std::vector<ImagePart>& parties, Correspondance& meilleurCorrespondance) const {
	/* Cherche la meilleur image d'origine pour une transformation
	 * Entrée :
	 *   - parties : un tableau de bouts d'images
	 *   - meilleurCorrespondance : un type Correspondance :
	 *   	- bloc : l'indice du bout d'image choisis dans "parties"
	 *   	- transformation : la transformation optimale pour ce bloc
	 * Sortie : true si la variance est considérée comme correcte
	 */
	extern float SEUIL_LISSAGE, SEUIL_VARIANCE;
	extern int SEUIL_DECOUPE;

	int n = parties.size();
	float varianceMin, variance;
	Transformation transfo = chercherTransformation(parties[0], varianceMin); // Donne une valeur initiale à varianceMin

	meilleurCorrespondance.transformation = transfo;
	meilleurCorrespondance.bloc = 0;
	meilleurCorrespondance.spliter  = 0;

	// On fait une recherche de minimum sur la variance
	for(int i=1 ; i<n && (varianceMin>SEUIL_LISSAGE || meilleurCorrespondance.transformation.droite.a != 0) && varianceMin>SEUIL_VARIANCE ; i++) {
		transfo = chercherTransformation(parties[i], variance);
		if(varianceMin > variance) {
			meilleurCorrespondance.bloc = i;
			meilleurCorrespondance.transformation = transfo;
			varianceMin = variance;
		}
	}

	return varianceMin > SEUIL_DECOUPE;
}

float ImagePart::varianceDifference(const ImagePart& B, LinReg *ldroite, bool regression) const {
	/* Compare deux images :
	 * Etudie la variance des "distances" entre les pixels
	 * La moyenne de chaque image est ajustée par ajout d'une constante
	 * Entrées :
	 *  - B : l'image à laquelle this est comparé
	 *  - decalage : un pointeur sur une variable qui prendra la valeur de la régression linéaire appliquée
	 *     -> decalage doit être appliqué à B pour qu'il ressemble à l'objet courrant
	 *   - regression : doit faire une regression linéaire ?
	 *     -> sinon il utilise celle passée en argument
	 */
	LinReg droite;
	if(regression) droite = chercherLinReg(B); // On cherche une transfo linéaire
	else droite = *ldroite;

	float sumCarre = 0;
	float somme = 0; // Pour calculer la moyenne
	for(int i=0 ; i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			int ecart = couleurLinReg(droite, B.at(i, j)) - at(i, j); // On utilise la régression linéaire
			sumCarre += std::pow(ecart, 2);
			somme += std::abs(ecart);
		}
	}
	if(ldroite != NULL) *ldroite = droite;

	float n = mTaille*mTaille;
	float moyenne = somme/n;
	return (sumCarre/n) - (moyenne*moyenne);
}

std::queue<ImagePart> ImagePart::spliter() const {
	/* Découpe la partie d'image en 4 sous-parties
	 * /!\ Pour une entrée de taille impaire le pixel du milieu sera pris en compte deux fois
	 * Si la découpe échoue, la valeur de retour est cet objet
	 */
	int nvlleTaille = mTaille / 2;
	int midX = mX + nvlleTaille;
	int midY = mY + nvlleTaille;
	std::queue<ImagePart> retour;
		if(mTaille == 1) {
			DEBUG << "Tentative de découpe de taille 1" << std::endl;
			retour.push(*this);
			return retour;
		}
		retour.push(ImagePart(mImage, mX, mY, nvlleTaille));
		retour.push(ImagePart(mImage, midX, mY, mTaille - nvlleTaille));
		retour.push(ImagePart(mImage, mX, midY, mTaille - nvlleTaille));
		retour.push(ImagePart(mImage, midX, midY, nvlleTaille));
	return retour;
}

void ImagePart::sauvegarder(const char* fichier) const {
	/* Enregistre le bout d'image au format png */
	ImageMatricielle image(mTaille, mTaille);
	for(int i=0 ; i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			image[i][j] = at(i, j);
		}
	}
	image.sauvegarder(fichier);
}

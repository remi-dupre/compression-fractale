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
    /* Point a picture part.
     * Inputs:
     * - mom: a pointer to the source image
     * - x, y: the coordinates of the top-left point of the square
     * - size: the dimension of the square
     */
	mVirtuel = false;
}

ImagePart::ImagePart(int taille) : mTaille(taille) {
	/* Crée un "faux" bout d'image.
	 * Entrées :
	 *   - taille : la cote du carré
	 */
    /* Create a "fake" piece of picture.
     * Inputs:
     * - size: the dimension of the square
    */
	mVirtuel = true;
	mX = mY = 0;
	mImage = new ImageMatricielle(mTaille, mTaille);
}

ImagePart::~ImagePart() {
	if(mVirtuel) { // L'image source a été créée dans le constructeur // The source image was created in the constructor
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
    /* Modify the value of an element in the parts
     * /! \ Prohibits changes from outside
     * Inputs:
     * - i, j: the coordinates of the pixel to be modified, in [0, size [
     * - val: the value to assign
     */
	if( ( i>=0 && i<mTaille ) &&  ( j>=0 && j<mTaille ) ) // Ca ne dépasse pas de la parcelle  //  It does not exceed the parcel
	if( i+mX < mImage->getLargeur() && j+mY < mImage->getHauteur() ) // ca ne dépasse pas de l'image  // it does not go beyond the image
		(*mImage)[i+mX][j+mY] = val;
}

unsigned char ImagePart::at(int i, int j) const {
	/* Retourne la valeur aux coordonnées données (i,j)
	 * Si les coordonnées dépassent du blocs mais restent dans l'image ça marche quand même
	 */
    /* Returns the value at the given coordinates (i, j)
     * If the coordinates exceed the block but remain in the image it still works
     */
	int ix = i+mX;
	int jy = j+mY;

	if(ix < 0) ix = 0; // Si ca dépasse on se projete sur le bord  // If it goes over, we'll be on the edge
	else if(ix >= mImage->getLargeur()) ix = mImage->getLargeur() - 1;

	if(jy < 0) jy = 0; // Si ca dépasse on se projete sur le bord  // If it goes over, we'll be on the edge
	else if(jy >= mImage->getHauteur()) jy = mImage->getHauteur() - 1;

	return (*mImage)[ix][jy];
}

int ImagePart::getTaille() const { return mTaille; } // La cote du carré  // The dimension of the square

unsigned char ImagePart::couleurMoyenne() const {
	/* La moyenne des couleurs représentées sur le bout d'image */
    /* The average of the colors represented on the end of image */
	unsigned int somme = 0;
	for(int i=0 ; i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			somme += at(i, j);
		}
	}
	return somme/(mTaille*mTaille);
}

void ImagePart::remplir(unsigned char couleur) {
	/* Remplis le bout d'image avec une couleur uniforme // Fill the image end with a uniform color */
	for( int i = 0 ; i < mTaille ; i++ ) {
	for( int j = 0 ; j < mTaille ; j++ ) {
			set(i, j, couleur);
		}
	}
}

void ImagePart::encadrer() {
	/* Crée un bord noir et un milieu blanc // Creates a black edge and a white background
	*/
	for( int i = 0 ; i < mTaille ; i++ ) {
		bool bordH = ( i == 0 ) || ( i == mTaille - 1 );
		for( int j = 0 ; j < mTaille ; j++ ) {
			bool bordV = ( j == 0 ) || ( j == mTaille - 1 );
			int couleur = ( bordH || bordV ) ? 0 : 255;
			set(i, j, couleur);
		}
	}
}

/* *************** Régression linéaire *************** */

LinReg ImagePart::chercherLinReg(const ImagePart& X) const {
	/* On cherche une fonction affine f telle que f(X) ~= Y
	 * Les formules développées viennent de wikipédia et sont démontrable avec le programme de supp
	 */
    /* We search for an affine function f such that f (X) ~ = Y
      * The developed formulas come from wikipedia and are demonstrable with the program of supp
    */
	const ImagePart &Y = *this;
	float sumX, sumY, sumXY, sumXX; // On a besoins de calculer 4 grosses sommes // We need to calculate 4 large sums
	sumX = sumY = sumXY = sumXX = 1; // On évite les divisions par 0  // Divisions by Zero are avoided
	for(int i=0 ;  i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			sumX += X.at(i, j);
			sumY += Y.at(i, j);
			sumXY += X.at(i, j) * Y.at(i, j);
			sumXX += std::pow(X.at(i, j), 2);
		}
	}
	float n = mTaille*mTaille;
	LinReg retour;
		retour.a = ( (sumX*sumY/n) - sumXY ) / ( (sumX*sumX/n) - sumXX );
		retour.a = decode16bFloat( Flotant16b( retour.a ) );
		retour.b = ( sumY - (retour.a*sumX) ) / n;
	return retour;
}

void ImagePart::appliquerLinReg(const LinReg& droite) {
	/* Applique la fonction affine droite à tous les pixels de la parcelle  // Applies the right affine function to all pixels in the parcel */
	for(int i = 0 ; i < mTaille ; i++) {
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
    /* Apply a linear transformation on the image part
     * Inputs:
     * - imgExit: the end of image in which one records the result of the transformation
     * - transformer: a type of transformation
     * /! \ It is better to accompany this function with a rough draft of calculations
    */
	int a = imgSortie.getTaille();
	float grandissement = float(mTaille) / a;
	float rapportx = cos(RAD(transfo.rotation)); // r*e^(i*theta)
	float rapporty = sin(RAD(transfo.rotation));
	float centre = a / 2; // Centre de la rotation, en x et en y // Center of the rotation, in x and y

	int is, js;
	for(is=0 ; is<a ; is++) {
		for(js=0 ; js<a ; js++) {
			int i = rint( grandissement* (rapportx*(is-centre) - rapporty*(js-centre) + centre) ); // Calculs des parties imaginaires et réelles // Calculations of imaginary and real parts
			int j = rint( grandissement* (rapporty*(is-centre) + rapportx*(js-centre) + centre) );
			imgSortie.set(is, js, couleurLinReg(transfo.droite, at(i, j))); // On a trouvé le point correspondant, on rajoute le décalage de couleur // We found the corresponding point, we add the color shift
		}
	}
}

float ImagePart::chercherTransformation(const ImagePart& origine, Transformation& min) const {
	/* Cherche la meilleur transformation de origine pour correspondre à cet objet
	 * Entrées :
	 *   - origine : l'image qui subis les transformations
	 *   - min : un type Transformation
	 * Sorties :
	 *   - retourne la variance correspondant à la transformation trouvée
	 *   - modifie min, la transformation trouvée
	 */
    /* Look for the best original transformation to match this object
      * Inputs:
      * - origin: the image that undergoes transformations
      * - min: a type Transformation
      * Exits :
      * - returns the variance corresponding to the transformation found
      * - modifies min, the transformation found
      */
	extern float SEUIL_LISSAGE, SEUIL_VARIANCE;

	Transformation max = ROTATION(360); // max sert juste de borne mais ne peut pas être la valeur de retour // max just serves as a bound but can not be the return value
	Transformation mid = ROTATION(0);

	min.rotation = 0;
	min.droite.a = 0;					// Vérifie la fonction constante  // Check the constant function
	min.droite.b = couleurMoyenne();	// Donne la couleur exacte sur les bouts lisses, crée de la redondance  // Gives the exact color on smooth ends, creates redundancy

	ImagePart img(mTaille);

	origine.transformer(img, max);
	float varmax = moyenneDifference(img); // max sert juste de borne  //  max is just a terminal

	origine.transformer(img, min);
	float varmin = moyenneDifference(img, &min.droite, false); // Il faut donner une valeur à min.droite au cas où il est retourné // You must give a value to min.right in case it is returned

	/* Application de la dichotomie :
	 * La variance en fonction de l'angle n'est pas (/rarement) monotone, l'algorithme tend vers un minimum local
	 * A chaque itération on choisis l'angle moitié, puis mon garde l'angle de variance la plus faible et ce nouvel angle
	 *  -> min < max gardent leur ordre mais le qualificatif n'est relatif qu'à leur angle
	 *  -> de toutes facons, à la fin varmin ~= varmax
	 */
    /* Application of the dichotomy:
     * The variance as a function of the angle is not (/ rarely) monotonous, the algorithm tends towards a local minimum
     * At each iteration we choose the angle half, then my guard the angle of variance the weakest and this new angle
     * -> min <max keep their order but the qualifier is only relative to their angle
     * -> anyway, in the end varmin ~ = varmax
   */
	LinReg droite;
	if(varmin > SEUIL_LISSAGE) {
		while(max.rotation - min.rotation > 5 && varmin > SEUIL_VARIANCE) {
			mid.rotation = (max.rotation + min.rotation) / 2; // On prend le milieu et on calcul la transformation  // Take the middle and calculate the transformation
			origine.transformer(img, mid);
			float variance = moyenneDifference(img, &droite);
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
	return varmin; // On retourne la variance obtenue pour éviter de refaire le calcul // We return the variance obtained to avoid redoing the calculation
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
    /* Look for the best original image for a transformation
      * Entrance :
      * - parts: a picture of pieces of pictures
      * - bestCorrespondance: a type Correspondence:
      * - block: the index of the image end chosen in "parts"
      * - transformation: the optimal transformation for this block
      * Output: true if the variance is considered correct
     */
	extern float SEUIL_LISSAGE, SEUIL_VARIANCE;
	extern float SEUIL_DECOUPE;

	int n = parties.size();
	Transformation transfo;
	float variance, varianceMin = chercherTransformation(parties[0], transfo); // Donne une valeur initiale à varianceMin // Gives an initial value to varianceMin

	meilleurCorrespondance.transformation = transfo;
	meilleurCorrespondance.bloc = 0;
	meilleurCorrespondance.spliter  = 0;

	// On fait une recherche de minimum sur la variance  // We do a minimal search on the variance
	for(int i=1 ; i<n && (varianceMin>SEUIL_LISSAGE || meilleurCorrespondance.transformation.droite.a != 0) && varianceMin>SEUIL_VARIANCE ; i++) {
		variance = chercherTransformation(parties[i], transfo);
		if(varianceMin > variance) {
			meilleurCorrespondance.bloc = i;
			meilleurCorrespondance.transformation = transfo;
			varianceMin = variance;
		}
	}

	return varianceMin < SEUIL_DECOUPE;
}

float ImagePart::moyenneDifference(const ImagePart& B, LinReg *droite, bool regression) const {
	/* Compare deux images :
	 * Etudie la moyenne des "distances" entre les pixels
	 * La moyenne de chaque image est ajustée par régression linéaire
	 * Entrées :
	 *  - B : l'image à laquelle this est comparé
	 *  - decalage : un pointeur sur une variable qui prendra la valeur de la régression linéaire appliquée
	 *     -> decalage doit être appliqué à B pour qu'il ressemble à l'objet courrant
	 *   - regression : doit faire une regression linéaire ?
	 *     -> sinon il utilise celle passée en argument (ne doit pas être NULL)
	 */
    /* Compare two images:
       * Study the average of the "distances" between the pixels
       * The average of each image is adjusted by linear regression
       * Inputs:
       * - B: the image to which this is compared
       * - offset: a pointer to a variable that will take the value of the applied linear regression
       * -> Offset must be applied to B so that it looks like the current object
       * - regression: must do a linear regression?
       * -> else it uses the one passed in argument (must not be NULL)
     */
	bool droiteToDestroy = false; // true s'il faut désalouer la valeur de "droite" // true if you have to unset the value of "right"
	if(droite == NULL) {
		droite = new LinReg();
		droiteToDestroy = true;
	}

	if(regression)
		*droite = chercherLinReg(B); // On cherche une transfo linéaire // We are looking for a linear transformer

	float sumCarre = 0;
	for(int i=0 ; i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			int ecart = couleurLinReg(*droite, B.at(i, j)) - at(i, j); // On utilise la régression linéaire // Linear regression is used
			sumCarre += std::pow(ecart, 4);
		}
	}

	if( droiteToDestroy ) delete droite; // Droite valait NULL // Right was NULL
	float n = mTaille*mTaille;
	return sumCarre / n;
}

std::queue<ImagePart> ImagePart::spliter() const {
	/* Découpe la partie d'image en 4 sous-parties
	 * /!\ Pour une entrée de taille impaire le pixel du milieu sera pris en compte deux fois
	 * Si la découpe échoue, la valeur de retour est cet objet
	 */
    /* Cut the image part into 4 subparts
     * /! \ For odd-size input the middle pixel will be counted twice
     * If the cut fails, the return value is this object
    */
	int nvlleTaille = mTaille / 2;
	int midX = mX + nvlleTaille;
	int midY = mY + nvlleTaille;
	std::queue<ImagePart> retour;
		if(mTaille == 1) {
			DEBUG << "Attempt to cut size 1" << std::endl;
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
    /* Save the image end in png format */
	ImageMatricielle image(mTaille, mTaille);
	for(int i=0 ; i<mTaille ; i++) {
		for(int j=0 ; j<mTaille ; j++) {
			image[i][j] = at(i, j);
		}
	}
	image.sauvegarder(fichier);
}

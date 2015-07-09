#include "ImageMatricielle.h"

/* *************** Constructeur / Destructeur *************** */

ImageMatricielle::ImageMatricielle(unsigned int x, unsigned int y) : mLargeur(x), mHauteur(y) {
	/* Créée une nouvelle image de dimensions données
	 * Les pixels de l'image ne sont pas initialisés
	 */
	mImage = new unsigned int* [mLargeur];
	for(int i=0 ; i<mLargeur ; i++) {
		mImage[i] = new unsigned int[mHauteur];
	}
}

ImageMatricielle::ImageMatricielle(const char* fichier, int couche) {
	/* Ouvre un fichier '.png'
	 * Entrées :
	    - fichier : l'adresse du fichier
		- couche : la couche à lire (de 0 à 3)
	 * Sortie : si l'ouverture échoue, l'image prend les dimensions 0x0
	 */
	bool erreur = false;
	std::vector<unsigned char> png;
	std::vector<unsigned char> img;

	lodepng::load_file(png, fichier	);
	unsigned error = lodepng::decode(img, mLargeur, mHauteur, png);

	if(error) {
		std::cerr << fichier << " -> png decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		mLargeur = mHauteur = 0;
		erreur = true;
	}

	mImage = new unsigned int* [mLargeur];
	for(int i=0 ; i<mLargeur ; i++) {
		mImage[i] = new unsigned int[mHauteur];
		for(int j=0 ; j<mHauteur ; j++) {
			mImage[i][j] = img[ (i + j*mLargeur)*4 + couche ];
		}
	}

	if(!erreur) std::cout << "image lue : " << fichier << " (" << mLargeur << "x" << mHauteur << "px) : couche " << couche << std::endl;
}

ImageMatricielle::~ImageMatricielle() {
	/* Suppression de l'image */
	for(int i=0 ; i<mLargeur ; i++) {
		delete[] mImage[i];
	}
	delete[] mImage;
}

/* *************** Setters / Getters *************** */

unsigned int* ImageMatricielle::operator[](int i) {
	/* Retourne la ligne de l'image correspondante */
	return mImage[i];
}

unsigned int ImageMatricielle::getHauteur() const { return mHauteur; }
unsigned int ImageMatricielle::getLargeur() const { return mLargeur; }

/* *************** Compression *************** */

std::vector<ImagePart> ImageMatricielle::decouper(int taille) {
	/* Découpe l'image en sous-images
	 * Les sous parties sont des carrés de côté "taille", le dépassement est ignoré
	 */
	std::vector<ImagePart> liste;
	for(int i=0 ; i*taille<mLargeur ; i++) {
		for(int j=0 ; j*taille<mHauteur ; j++) {
			liste.push_back( ImagePart(this, i*taille, j*taille, taille) );////////////////////////////////////////////// !!!!!!
		}
	}
	return liste;
}

std::vector<Source> ImageMatricielle::compresser(unsigned int taillePetit, unsigned int tailleGros) {
	/* Effectue la compression de l'image
	 * Entrées :
	 *  - taillePetit : la taille des blocs du petit pavages
	 *  - tailleGros : taille des gros blocs, doit être plus grand que taillePetit
	 */
	if(taillePetit >= tailleGros) {
		std::cout << "Le pavage n'est pas de la bonne dimension" << std::endl;
		return std::vector<Source>();
	}
	std::cout << "Compression initiée" << std::endl;

	std::cout << " 1 - Création des pavages" << std::endl;
	std::vector<ImagePart> pavagePetit = decouper(taillePetit);
	std::vector<ImagePart> pavageGros = decouper(tailleGros);
	std::cout << "   - " << pavagePetit.size() << " et " << pavageGros.size() << " blocs" << std::endl;

	std::vector<Source> correspondances;
	for(int i=0 ; i<pavagePetit.size() ; i++) {
		chargement(" 2 - Recherche des correspondances", i, pavagePetit.size());
		correspondances.push_back( pavagePetit[i].chercherMeilleur(pavageGros) );
	}
	chargement(" 2 - Recherche des correspondances", pavagePetit.size(), pavagePetit.size());
	std::cout << "\n";

	return correspondances;
}

#include "ImageMatricielle.h"

/* *************** Constructeur / Destructeur *************** */

ImageMatricielle::ImageMatricielle(unsigned int x, unsigned int y) : largeur(x), hauteur(y) {
	/* Créée une nouvelle image de dimensions données
	 * Les pixels de l'image ne sont pas initialisés
	 */
	image = new unsigned int* [largeur];
	for(int i=0 ; i<largeur ; i++) {
		image[i] = new unsigned int[hauteur];
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
	unsigned error = lodepng::decode(img, largeur, hauteur, png);

	if(error) {
		std::cout << fichier << " -> png decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		largeur = hauteur = 0;
		erreur = true;
	}

	image = new unsigned int* [largeur];
	for(int i=0 ; i<largeur ; i++) {
		image[i] = new unsigned int[hauteur];
		for(int j=0 ; j<hauteur ; j++) {
			image[i][j] = img[ (i + j*largeur)*4 + couche ];
		}
	}

	if(!erreur) std::cout << "image lue : " << fichier << " (" << largeur << "x" << hauteur << "px) : couche " << couche << std::endl;
}

ImageMatricielle::~ImageMatricielle() {
	/* Suppression de l'image */
	for(int i=0 ; i<largeur ; i++) {
		delete[] image[i];
	}
	delete[] image;
}

/* *************** Setters / Getters *************** */

unsigned int* ImageMatricielle::operator[](int i) {
	/* Retourne la ligne de l'image correspondante */
	return image[i];
}

unsigned int ImageMatricielle::getHauteur() const { return hauteur; }
unsigned int ImageMatricielle::getLargeur() const { return largeur; }

/* *************** Compression *************** */

std::vector<ImagePart>* ImageMatricielle::decouper(int taille) {
	/* Découpe l'image en sous-images
	 * Les sous parties sont des carrés de côté "taille", le dépassement est ignoré
	 */
	std::vector<ImagePart>* liste = new std::vector<ImagePart>();
	for(int i=0 ; i*taille<largeur ; i++) {
		for(int j=0 ; j*taille<hauteur ; j++) {
			liste->push_back( ImagePart(this, i*taille, j*taille, taille) );////////////////////////////////////////////// !!!!!!
		}
	}
	return liste;
}

void ImageMatricielle::compresser(unsigned int taillePetit, unsigned int tailleGros) {
	/* Effectue la compression de l'image, pas de retour le temps de savoir quoi faire
	 * Entrées :
	 *  - taillePetit : la taille des blocs du petit pavages
	 *  - tailleGros : taille des gros blocs, doit être plus grand que taillePetit
	 */
	if(taillePetit >= tailleGros) {
		std::cout << "Le pavage n'est pas de la bonne dimension" << std::endl;
		return;
	}
	std::cout << "Compression initiée" << std::endl;

	std::cout << " 1 - Création des pavages" << std::endl;
	std::vector<ImagePart>* pavagePetit = decouper(taillePetit);
	std::vector<ImagePart>* pavageGros = decouper(tailleGros);
	std::cout << "   - " << pavagePetit->size() << " et " << pavageGros->size() << " blocs" << std::endl;

	std::vector<Source>* correspondances = new std::vector<Source>();
	for(int i=0 ; i<pavagePetit->size() ; i++) {
		chargement(" 2 - Recherche des correspondances", i, pavagePetit->size());
		correspondances->push_back( (*pavagePetit)[i].chercherMeilleur(*pavageGros) );
	}
	chargement(" 2 - Recherche des correspondances", pavagePetit->size(), pavagePetit->size());
	std::cout << "\n";

}

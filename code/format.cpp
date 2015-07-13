#include "format.h"

int couleurLinReg(const LinReg& droite, int couleur) {
    /* Donne l'image d'une couleur par une fonction affine
     *  - droite : la fonction affine
     *  - couleur : la couleur
     */
    return int(std::min(255, std::max(0, int(droite.a*couleur + droite.b))));
}

ImageFractale compresser(const char* fichier, int precisionPetit, int precisionGros, bool couleur, bool transparence) {
	ImageMatricielle image(fichier, 0);
	ImageFractale retour;
		retour.largeur = image.getLargeur();
		retour.hauteur = image.getHauteur();
		retour.couleur = couleur;
		retour.transparence = transparence;
		if(couleur) {
			for(int i=0 ; i<3 ; i++) {
			ImageMatricielle imageTr(fichier, i);
			retour.ifs.push_back( imageTr.chercherIFS(precisionPetit, precisionGros) );
			retour.moyenne.push_back( imageTr.moyenne() );
			}
		}
		else {
			retour.ifs.push_back( image.chercherIFS(precisionPetit, precisionGros) );
			retour.moyenne.push_back( image.moyenne() );
		}
		if(transparence) {
			ImageMatricielle imageTr(fichier, 3);
			retour.ifs.push_back( imageTr.chercherIFS(precisionPetit*2, precisionGros*2) );
			retour.moyenne.push_back( imageTr.moyenne() );
		}
	return retour;
}

void sauvegarder(const char* fichier, const ImageFractale& imgF) {
	std::vector<ImageMatricielle*> couche(imgF.ifs.size(), NULL);
	for(int i=0 ; i < imgF.ifs.size() ; i++) {
		couche[i] = new ImageMatricielle(imgF.largeur, imgF.hauteur);
		couche[i]->remplir(imgF.moyenne[i]);
		for(int k=0 ; k<20 ; k++) {				
			ImageMatricielle *nouveau =  new ImageMatricielle(couche[i]->appliquerIFS(imgF.ifs[i]));
			delete couche[i]; // On désaloue pour ne pas créer de fuite de mémoire
			couche[i] = nouveau;
		}
	}
	
	std::vector<unsigned char> pixels;
	for(int j=0 ; j<imgF.hauteur ; j++) {
		for(int i=0 ; i<imgF.largeur ; i++) {
			if(imgF.couleur) {
				for(int n=0 ; n<3 ; n++) pixels.push_back( (unsigned char)( (*couche[n])[i][j] ) );
			}
			else{
				for(int n=0 ; n<3 ; n++) pixels.push_back( (unsigned char)( (*couche[0])[i][j] ) );
			}
			if(imgF.transparence) pixels.push_back( (unsigned char)( (*couche[couche.size()-1])[i][j] ) );
			else pixels.push_back(255);
		}
	}

	std::vector<unsigned char> png;
	unsigned error = lodepng::encode(png, pixels, imgF.largeur, imgF.hauteur);
	if(!error) lodepng::save_file(png, fichier);

	if(error) std::cout << fichier << " -> encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}
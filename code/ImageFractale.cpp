#include "ImageFractale.h"

void ImageFractale::exporter(const char* fichier) {
    /* Crée un rendu de l'image et l'exporte au format (.png) */
	std::vector<ImageMatricielle*> couche(mIfs.size(), NULL);
	for(int i=0 ; i < mIfs.size() ; i++) {
		couche[i] = new ImageMatricielle(mLargeur, mHauteur);
		couche[i]->remplir(mMoyenne[i]);
		for(int k=0 ; k<20 ; k++) {
			ImageMatricielle *nouveau =  new ImageMatricielle(couche[i]->appliquerIFS(mIfs[i]));
			delete couche[i]; // On désaloue pour ne pas créer de fuite de mémoire
			couche[i] = nouveau;
		}
	}

	std::vector<unsigned char> pixels;
	for(int j=0 ; j<mHauteur ; j++) {
		for(int i=0 ; i<mLargeur ; i++) {
			if(mCouleur) {
				for(int n=0 ; n<3 ; n++) pixels.push_back( (unsigned char)( (*couche[n])[i][j] ) );
			}
			else{
				for(int n=0 ; n<3 ; n++) pixels.push_back( (unsigned char)( (*couche[0])[i][j] ) );
			}
			if(mTransparence) pixels.push_back( (unsigned char)( (*couche[couche.size()-1])[i][j] ) );
			else pixels.push_back(255);
		}
	}

	std::vector<unsigned char> png;
	unsigned error = lodepng::encode(png, pixels, mLargeur, mHauteur);
	if(!error) lodepng::save_file(png, fichier);

	if(error) std::cout << fichier << " -> encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

ImageFractale ImageFractale::compresser(const char* fichier, int precisionPetit, int precisionGros, bool couleur, bool transparence) {
    /* Retourne la compression d'un fichierau format ImageFractale
     * Entrées :
     *  - fichier : l'adresse du fichier (.png) à lire
     *  - precisionPetit : la taille des petits blocs
     *     | Réduire cette grandeur augmente la qualité
     *     | Réduire cette grandeur augmentera la taille du fichier final
     *  - precisionGros : la taille des gros blocs
     *     | /!\ Doit être plus grand que 'taillePetit' (recommandation : 150%)
     *     | Réduire cette grandeur augmente la qualitéen général (cf. recommandation)
     *     | Réduire cette grandeur augmente la durée de calculs
     *  - couleur : true si l'image est en couleur (x3 temps de calculs)
     *  - transparence : true s'il y a une couche alpha
     */
	ImageMatricielle image(fichier, 0);
	ImageFractale retour;
		retour.mLargeur = image.getLargeur();
		retour.mHauteur = image.getHauteur();
		retour.mCouleur = couleur;
		retour.mTransparence = transparence;
		if(couleur) {
			for(int i=0 ; i<3 ; i++) {
			ImageMatricielle imageTr(fichier, i);
			retour.mIfs.push_back( imageTr.chercherIFS(precisionPetit, precisionGros) );
			retour.mMoyenne.push_back( imageTr.moyenne() );
			}
		}
		else {
			retour.mIfs.push_back( image.chercherIFS(precisionPetit, precisionGros) );
			retour.mMoyenne.push_back( image.moyenne() );
		}
		if(transparence) {
			ImageMatricielle imageTr(fichier, 3);
			retour.mIfs.push_back( imageTr.chercherIFS(precisionPetit*2, precisionGros*2) );
			retour.mMoyenne.push_back( imageTr.moyenne() );
		}
	return retour;
}

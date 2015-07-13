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

/* *************** Enregistrement *************** */

struct Pack_Entete {
	unsigned int largeur:15;
	unsigned int hauteur:15;
	bool couleur:1;
	bool transparence:1;
};

struct Pack_IFS { // 4 octects
	unsigned short int decoupeGros;
	unsigned short int decoupePetit;
};

struct Pack_Correspondance { // 14 + 9 + 9 + 16 = 48 = 6*8 octects
	unsigned int bloc		:14; // Jusqu'a 16383 blocs
	unsigned int rotation	:9;	 // 511°
	signed short int b		:9;	 // -255 à 256
	signed short int a:16,	:16; // 16 bits inutiles à la fin
};

void ImageFractale::enregistrer(const char* fichier) const {
	std::ofstream f(fichier, std::ios::trunc | std::ios::binary);
	if (!f.is_open()) std::cout << "Impossible d'ouvrir le fichier en lecture !" << std::endl;

	Pack_Entete entete;
		entete.largeur = mLargeur;
		entete.hauteur = mHauteur;
		entete.couleur = mCouleur;
		entete.transparence = mTransparence;
	f.write((char*)&entete, sizeof(Pack_Entete));

	for(int i=0 ; i < mIfs.size() ; i++) {
		Pack_IFS ifs;
			ifs.decoupeGros = mIfs[i].decoupeGros;
			ifs.decoupePetit = mIfs[i].decoupePetit;
		f.write((char*)&ifs, sizeof(Pack_IFS));

		const std::vector<Correspondance> &mCorrespondances = mIfs[i].correspondances;
		for(int j=0 ; j < mCorrespondances.size() ; j++) {
			Pack_Correspondance correspondance;
				correspondance.bloc = mCorrespondances[j].bloc;
				correspondance.rotation = mCorrespondances[j].transformation.rotation;
				correspondance.a = mCorrespondances[j].transformation.droite.a*100;
				correspondance.b = mCorrespondances[j].transformation.droite.b;
			f.write((char*)&correspondance, 6); // ! \\ Le 6 peut varier avec un changement de structure
		}
	}

	f.close();
}

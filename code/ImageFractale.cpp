#include "ImageFractale.h"

/* *************** Ouverture *************** */

ImageFractale::ImageFractale() {
	mLargeur = mHauteur = 0;
	mCouleur = mTransparence = false;
	mMoyenne = std::vector<unsigned char>();
	mIfs = std::vector<IFS>();
}

ImageFractale::ImageFractale(const char* fichier) : mIfs(std::vector<IFS>()), mMoyenne(std::vector<unsigned char>()) {
	std::ifstream f(fichier, std::ios::in | std::ios::binary);

	Pack_Entete entete;
	f.read((char*)&entete, sizeof(Pack_Entete));
		mLargeur = entete.largeur;
		mHauteur = entete.hauteur;
		mCouleur = entete.couleur;
		mTransparence = entete.transparence;

	int nbCouches = mCouleur ? 3 : 1;
	if(mTransparence) nbCouches += 1;

	for(int i=0 ; i<nbCouches ; i++) {
		Pack_IFS ifs;
		mIfs.push_back(IFS());
		f.read((char*)&ifs, sizeof(Pack_IFS));
			mIfs[i].decoupeGros = ifs.decoupeGros;
			mIfs[i].decoupePetit = ifs.decoupePetit;
		mMoyenne.push_back( ifs.moyenne );

		int nbBlocs = std::ceil((float)mLargeur/ifs.decoupePetit) * std::ceil((float)mHauteur/ifs.decoupePetit);
		for(int j=0 ; j < nbBlocs ; j++) {
			Pack_Correspondance correspondance;
			Correspondance mCorrespondance;
			f.read((char*)&correspondance, 4); // ! \\ Le 6 peut varier avec un changement de structure
				mCorrespondance.bloc = correspondance.bloc;
				mCorrespondance.transformation.rotation = correspondance.rotation * 360 / 255;
				mCorrespondance.transformation.droite.b = correspondance.b;

			Flotant16b a;
			f.read((char*)&a, 2); // ! \\ Le 6 peut varier avec un changement de structure
			mCorrespondance.transformation.droite.a = decode16bFloat(a);

			mIfs[i].correspondances.push_back( mCorrespondance );
		}
	}

	f.close();
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

void ImageFractale::enregistrer(const char* fichier) const {
	std::ofstream f(fichier, std::ios::trunc | std::ios::binary);
	if (!f.is_open()) std::cout << "Impossible d'ouvrir le fichier en lecture !" << std::endl;

	Pack_Entete entete = packer_entete(*this);
	f.write((char*)&entete, sizeof(Pack_Entete));

	for(int i=0 ; i < mIfs.size() ; i++) {
		Pack_IFS ifs;
			ifs.decoupeGros = mIfs[i].decoupeGros;
			ifs.decoupePetit = mIfs[i].decoupePetit;
			ifs.moyenne = mMoyenne[i];
		f.write((char*)&ifs, sizeof(Pack_IFS));

		const std::vector<Correspondance> &mCorrespondances = mIfs[i].correspondances;
		for(int j=0 ; j < mCorrespondances.size() ; j++) {
			Pack_Correspondance correspondance;
				correspondance.bloc = mCorrespondances[j].bloc;
				correspondance.rotation = mCorrespondances[j].transformation.rotation * 255 / 360;
				correspondance.b = mCorrespondances[j].transformation.droite.b;
			f.write((char*)&correspondance, sizeof(Pack_Correspondance)); // ! \\ Le 6 peut varier avec un changement de structure

			Flotant16b a = (mCorrespondances[j].transformation.droite.a);
			f.write((char*)&a, 2); // ! \\ Le 6 peut varier avec un changement de structure
		}
	}

	f.close();
}

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

/* *************** Getters / Setters *************** */

int ImageFractale::getLargeur() const { return mLargeur; }
int ImageFractale::getHauteur() const { return mHauteur; }
bool ImageFractale::isCouleur() const { return mCouleur; }
bool ImageFractale::isTransparent() const { return mTransparence; }

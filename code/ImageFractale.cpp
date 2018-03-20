#include "ImageFractale.h"

/* *************** Ouverture *************** */

ImageFractale::ImageFractale() {
	mLargeur = mHauteur = 0;
	mCouleur = mTransparence = false;
	mMoyenne = std::vector<unsigned char>();
	mIfs = std::vector<IFS>();
}

ImageFractale::ImageFractale(const char* fichier) : mMoyenne(std::vector<unsigned char>()), mIfs(std::vector<IFS>()) {
	std::ifstream f(fichier, std::ios::in | std::ios::binary);

	Pack_Entete entete;
	f.read((char*)&entete, sizeof(Pack_Entete));
		mLargeur = entete.largeur;
		mHauteur = entete.hauteur;
		mCouleur = entete.couleur;
		mTransparence = entete.transparence;

	int nbCouches = mCouleur ? 3 : 1; // Calcul du nombre de couches // Calculation of the number of layers
	if(mTransparence) nbCouches += 1;

	for(int i=0 ; i<nbCouches ; i++) {
		Pack_IFS ifs;
		f.read((char*)&ifs, sizeof(Pack_IFS));
		mIfs.push_back( unpack_IFS(ifs) );
		mMoyenne.push_back( unpack_moyenne(ifs) );

		int nbBlocs = std::ceil((float)mLargeur/ifs.decoupePetit) * std::ceil((float)mHauteur/ifs.decoupePetit);
		lireCorrespondancesFichier(f, nbBlocs, mIfs[i].correspondances);
	}

	f.close();
}

ImageFractale ImageFractale::compresser(const char* fichier, int precisionPetit, int precisionGros, bool couleur, bool transparence) {
    /* Retourne la compression d'un fichier au format ImageFractale
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
    /* Returns compression of a file in ImageFractale format
     * Inputs:
     * - file: the address of the file (.png) to read
     * - precisionPetit: the size of small blocks
     * | Reducing this size increases the quality
     * | Reducing this size will increase the size of the final file
     * - precisionGros: the size of the big blocks
     * | /! \ Must be bigger than 'smallSize' (recommendation: 150%)
     * | Reducing this size increases quality in general (see recommendation)
     * | Reducing this size increases the calculation time
     * - color: true if the image is in color (x3 computation time)
     * - transparency: true if there is an alpha channel
     */
	ImageMatricielle image(fichier, 0);
	ImageFractale retour;
		retour.mLargeur = image.getLargeur();
		retour.mHauteur = image.getHauteur();
		retour.mCouleur = couleur;
		retour.mTransparence = transparence;
        
        
	COUT << "Size of the image : " << retour.mLargeur << "x" << retour.mHauteur << std::endl;

	if(couleur) {
		const char* message[] = {" - Red layer ", " - Green layer ", " - Blue layer "};
		for(int i=0 ; i<3 ; i++) {
			ImageMatricielle imageTr(fichier, i);
			retour.mIfs.push_back( imageTr.chercherIFS(precisionPetit, precisionGros, message[i]) );
			retour.mMoyenne.push_back( imageTr.moyenne() );
		}
	}
	else {
		retour.mIfs.push_back( image.chercherIFS(precisionPetit, precisionGros, " - NVDG layer  ") );
		retour.mMoyenne.push_back( image.moyenne() );
	}
	if(transparence) {
		ImageMatricielle imageTr(fichier, 3);
		retour.mIfs.push_back( imageTr.chercherIFS(precisionPetit, precisionGros, " - Alpha layer") );
		retour.mMoyenne.push_back( imageTr.moyenne() );
	}

	return retour;
}

/* *************** Enregistrement / Opening the file *************** */

void ImageFractale::enregistrer(const char* fichier) const {
	std::ofstream f(fichier, std::ios::trunc | std::ios::binary);
	if (!f.is_open()) std::cout << "Can't open the file '" << fichier << "'" << std::endl;

	Pack_Entete entete = packer_entete(*this); // Récupère les données binaires de l'en-tête // Retrieve the binary data from the header
	f.write((char*)&entete, sizeof(Pack_Entete));

	for(int i=0 ; i < mIfs.size() ; i++) {
		Pack_IFS ifs = packer_ifs(mIfs[i], mMoyenne[i]);
		f.write((char*)&ifs, sizeof(Pack_IFS));

		const std::vector<Correspondance> &mCorrespondances = mIfs[i].correspondances;
		std::cout << mCorrespondances.size() << std::endl;
		for(int j=0 ; j < mCorrespondances.size() ; j++) {
			Pack_Correspondance correspondance = packer_correspondance(mCorrespondances[j]);
			f.write((char*)&correspondance, SIZEOF_PACK_CORRESPONDANCE);
		}
	}

	f.close();
}

void ImageFractale::exporter(const char* fichier) {
    /* Crée un rendu de l'image et l'exporte au format (.png)
	*/
    /* Render the image and export it in (.png) format
    */
	extern int ITERATIONS_DECOMPRESSION, QUALITE_DECOMPRESSION;

	grandir(QUALITE_DECOMPRESSION);
    
	std::vector<ImageMatricielle*> couche((mIfs.size()), NULL);
    
    
	for(int i=0 ; i < ( mIfs.size()) ; i++) {
		couche[i] = new ImageMatricielle((mLargeur),(mHauteur));
		couche[i]->remplir(mMoyenne[i]);   // fill average
		for(int k=0 ; k < ITERATIONS_DECOMPRESSION ; k++) {
			ImageMatricielle *nouveau =  new ImageMatricielle(couche[i]->appliquerIFS(mIfs[i]));
			delete couche[i]; // On désaloue pour ne pas créer de fuite de mémoire // We do not want to create a memory leak
			couche[i] = nouveau;
		}
		couche[i]->lisser(QUALITE_DECOMPRESSION-1);
		couche[i]->retrecir(QUALITE_DECOMPRESSION);
	}

	retrecir(QUALITE_DECOMPRESSION);
   

	std::vector<unsigned char> pixels;
	for(int j=0 ; j<(mHauteur) ; j++) {
		for(int i=0 ; i<(mLargeur) ; i++) {
			if(mCouleur) {
				for(int n=0 ; n<3 ; n++) // Ajoute les 3 couleurs // Add the 3 colors
					pixels.push_back( (unsigned char)( (*couche[n])[i][j] ) );
			}
			else{
				for(int n=0 ; n<3 ; n++) // Pose 3 fois la même couleur  // Pose 3 times the same color
					pixels.push_back( (unsigned char)( (*couche[0])[i][j] ) );
			}
			if(mTransparence) pixels.push_back( (unsigned char)( (*couche[couche.size()-1])[i][j] ) );
			else pixels.push_back(255);
		}
	}

	std::vector<unsigned char> png;
	unsigned error = lodepng::encode(png, pixels, (mLargeur) , (mHauteur));
	if(!error) lodepng::save_file(png, fichier);

	if(error) std::cout << fichier << " -> png encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

void ImageFractale::debugSplit() const {
	extern const char* DOSSIER_DEBUG;
	for( int i = 0 ; i < mIfs.size() ; i++ ) {
		ImageMatricielle img(mLargeur, mHauteur);
		std::vector<ImagePart> parties = img.decouper( mIfs[i].decoupePetit );
		parties = ImageMatricielle::adapterDecoupe( parties, mIfs[i].correspondances );
		for( int j = 0 ; j < parties.size() ; j++ ) {
			parties[j].encadrer();
		}

		std::stringstream nom;
		nom << DOSSIER_DEBUG << "grille-" << i << ".png";
		img.sauvegarder( nom.str().c_str() );
	}
}

void ImageFractale::grandir(int grandissement) {
	mHauteur *= grandissement;
	mLargeur *= grandissement;
	for(int i=0 ; i < ( mIfs.size()) ; i++) {
		mIfs[i].decoupeGros *= grandissement;
		mIfs[i].decoupePetit *= grandissement;
	}
}

void ImageFractale::retrecir(int reduction) {
	(mHauteur) /= reduction;
	(mLargeur) /= reduction;
	for(int i=0 ; i < (mIfs.size()) ; i++) {
		mIfs[i].decoupeGros /= reduction;
		mIfs[i].decoupePetit /= reduction;
	}
}

/* *************** Getters / Setters *************** */

int ImageFractale::getLargeur() const { return mLargeur; }
int ImageFractale::getHauteur() const { return mHauteur; }
bool ImageFractale::isCouleur() const { return mCouleur; }
bool ImageFractale::isTransparent() const { return mTransparence; }

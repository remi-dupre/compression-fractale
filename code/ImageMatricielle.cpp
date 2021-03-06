#include "ImageMatricielle.h"

/* *************** Constructeur / Destructeur *************** */

ImageMatricielle::ImageMatricielle(unsigned int x, unsigned int y) : mLargeur(x), mHauteur(y) {
	/* Créée une nouvelle image de dimensions données
	 * Les pixels de l'image ne sont pas initialisés
	 */
	mImage = new unsigned char* [mLargeur];
	for(int i=0 ; i<mLargeur ; i++) {
		mImage[i] = new unsigned char[mHauteur];
	}
}

ImageMatricielle::ImageMatricielle(const char* fichier, int couche) {
	/* Ouvre un fichier '.png'
	 * Entrées :
	    - fichier : l'adresse du fichier
		- couche : la couche à lire (de 0 à 3)
	 * Sortie : si l'ouverture échoue, l'image prend les dimensions 0x0
	 */
	extern bool VERBOSE;

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

	mImage = new unsigned char* [mLargeur];
	for(int i=0 ; i<mLargeur ; i++) {
		mImage[i] = new unsigned char[mHauteur];
		for(int j=0 ; j<mHauteur ; j++) {
			mImage[i][j] = img[ (i + j*mLargeur)*4 + couche ];
		}
	}

	if(!erreur) DEBUG << "image lue : " << fichier << " (" << mLargeur << "x" << mHauteur << "px) : couche " << couche << std::endl;
}

ImageMatricielle::~ImageMatricielle() {
	/* Suppression de l'image */
	for(int i=0 ; i<mLargeur ; i++) {
		delete[] mImage[i];
	}
	delete[] mImage;
}

ImageMatricielle* ImageMatricielle::cloner() {
	ImageMatricielle* copie = new ImageMatricielle(mLargeur, mHauteur);
	for(int i=0 ; i < mLargeur ; i++) {
		for(int j=0 ; j < mHauteur ; j++) {
			(*copie)[i][j] = mImage[i][j];
		}
	}
	return copie;
}

/* *************** Setters / Getters *************** */

unsigned char* ImageMatricielle::operator[](int i) {
	/* Retourne la ligne de l'image correspondante */
	return mImage[i];
}

unsigned char& ImageMatricielle::at(int i, int j) {
	return mImage[ i % mLargeur ][ j % mHauteur ];
}

int ImageMatricielle::getHauteur() const { return mHauteur; }
int ImageMatricielle::getLargeur() const { return mLargeur; }

unsigned char ImageMatricielle::moyenne() const {
	/* Retourne la moyenne de teinte des pixels de l'image
	 */
	int somme = 0;
	for(int i=0 ; i<mLargeur ; i++) {
		for(int j=0 ; j<mHauteur ; j++) {
			somme += mImage[i][j];
		}
	}
	return somme/(mLargeur * mHauteur);
}

void ImageMatricielle::adapterMoyenne(unsigned char val) {
	/* Décale la moyenne de couleur des pixels pour la faire correspondre à 'val'
	 */
	int decalage = val - moyenne();
	for(int i=0 ; i<mLargeur ; i++) {
		for(int j=0 ; j<mHauteur ; j++) {
			mImage[i][j] += decalage;
		}
	}
}

void ImageMatricielle::remplir(unsigned char val) {
	/* Remplis l'image de la couleur val
	 */
	for(int i=0 ; i<mLargeur ; i++) {
		for(int j=0 ; j<mHauteur ; j++) {
			mImage[i][j] = val;
		}
	}
}

/* *************** Compression *************** */

std::vector<ImagePart> ImageMatricielle::decouper(int taille) {
	/* Découpe l'image en sous-images
	 * Les sous parties sont des carrés de côté "taille", le dépassement est ignoré
	 */
	std::vector<ImagePart> liste;
	for(int i=0 ; i*taille<mLargeur ; i++) {
		for(int j=0 ; j*taille<mHauteur ; j++) {
			liste.push_back( ImagePart(this, i*taille, j*taille, taille) );
		}
	}
	return liste;
}

std::vector<ImagePart> ImageMatricielle::adapterDecoupe(std::vector<ImagePart>& decoupe, const std::vector<Correspondance>& correspondances) {
	/* Adapte une découpe triviale à une liste de correspondances :
	 *  - les blocs à spliter vont se spliter
	 *  - la découpe de sortie sera de la même taille que la liste des correspondances
	 */
	std::list<ImagePart> aTraiter;
	for(int i=0 ; i < decoupe.size() ; i++) aTraiter.push_back(decoupe[i]); // On transforme l'entrée en liste
	std::vector<ImagePart> retour;
	int i = 0;

	while( !aTraiter.empty() ) {
		for(int k=0 ; k < correspondances[i].spliter ; k++) { // On splite le nombre de fois demandé
			std::queue<ImagePart> decoupes = aTraiter.front().spliter(); // Une file de 4 éléments
			aTraiter.pop_front();
			std::list<ImagePart>::iterator pos = aTraiter.begin(); // La position où on insère tout
			while( !decoupes.empty() ) {
				aTraiter.insert(pos, decoupes.front()); // On verse la découpe
				decoupes.pop();
			}
		}
		retour.push_back( aTraiter.front() );
		aTraiter.pop_front();
		i++;
	}
	return retour;
}

IFS ImageMatricielle::chercherIFS(int taillePetit, int tailleGros, const char* message) {
	/* Recherche l'ifs pour l'image
	 * Entrées :
	 *  - taillePetit : la taille des blocs du petit pavages
	 *  - tailleGros : taille des gros blocs, doit être plus grand que taillePetit
	 * Sortie : IFS
	 *  - correspondances : la liste (respectant les indinces des blocs) des 'Correspondance' a appliquer
	 *  - taillePetit / tailleGros : la taille de découpe
	 */
	extern int NB_THREADS;
	extern int NB_MAX_DECOUPE; // Permet de savoir combien de pavages créer

	int tDebut = time(0);
	if(taillePetit > tailleGros) {
		std::cerr << "Le pavage n'est pas de la bonne dimension" << std::endl;
		IFS retour;
			retour.correspondances = std::vector<Correspondance>();
			retour.decoupeGros = tailleGros;
			retour.decoupePetit = taillePetit;
		return retour;
	}

	COUT << "Création des pavages ...";
	std::vector<ImagePart> pavagePetit = decouper(taillePetit); // Les petits carrés dot on cherche une correspondance

	std::vector< std::vector<ImagePart> > pavagesGros;
	for(int i = 0 ; i <= NB_MAX_DECOUPE ; i++) {
		pavagesGros.push_back( decouper(tailleGros / std::pow(2, i)) ); // Adapte la découpe en fonction du niveau de récursion
	}
	DEBUG << "\rPetits pavés : " << pavagePetit.size() << ", Gros pavés : " << pavagesGros.size() << std::endl;

	std::vector< std::queue<ImagePart> > taches = decouperTache(pavagePetit, NB_THREADS); // Découpe les tâches
	std::vector< std::vector<Correspondance> > resultats(NB_THREADS, std::vector<Correspondance>() );
	std::vector<pthread_t> threads(NB_THREADS, pthread_t());
	std::vector<ThreadData> datas(NB_THREADS, ThreadData());
	for(int i=0 ; i<NB_THREADS ; i++) {
		datas[i].thread_id = i;
		datas[i].travail = taches[i];
		datas[i].antecedants = pavagesGros;
		datas[i].resultat = &resultats[i];
	}
	for(int i=0 ; i<NB_THREADS ; i++) {
		pthread_create(&threads[i], NULL, lancerThread, (void *)&datas[i]);
	}

	bool fini(false);
	while(!fini) {
		sleep(1);
		int avancement(pavagePetit.size());
		fini = true;
		for(int i=0 ; i<NB_THREADS ; i++) {
			avancement -= datas[i].travail.size();
			fini &= datas[i].travail.empty();
		}
        COUT << '\r' << message << chargement(avancement, pavagePetit.size(), 20);
	}

	std::vector<Correspondance> correspondances;
	for(int i=0 ; i<NB_THREADS ; i++) {
		for(int j=0 ; j<resultats[i].size() ; j++) {
			correspondances.push_back(resultats[i][j]);
		}
	}

	COUT << '\r' << message << ": " << pavagePetit.size() << '/' << pavagePetit.size() << " " << "(" << time(0) - tDebut << " secondes)" <<  std::endl;
	IFS retour;
		retour.correspondances = correspondances;
		retour.decoupeGros = tailleGros;
		retour.decoupePetit = taillePetit;
	debugIfs(retour);
	return retour;
}

ImageMatricielle ImageMatricielle::appliquerIFS(const IFS& ifs) {
	/* Applique l'IFS et en retourne le résultat
	 * Entrée : IFS : tout ce qui décrit une image encodée
	 * Sortie : l'image obtenue après application à cet objet
	 */
	extern int NB_MAX_DECOUPE;

	ImageMatricielle sortie(getLargeur(), getHauteur());
	std::vector< std::vector<ImagePart> > decoupesEntree;;
	for(int i = 0 ; i <= NB_MAX_DECOUPE ; i++) {
		decoupesEntree.push_back( decouper(ifs.decoupeGros / std::pow(2, i)) ); // Adapte la découpe en fonction des redécoupes
	}
	std::vector<ImagePart> decoupeSortie = sortie.decouper(ifs.decoupePetit);
	decoupeSortie = adapterDecoupe(decoupeSortie, ifs.correspondances);

	std::stack<int> dureeSplit; // Chaque élément représente une couche de redécoupe et le nombre d'éléments à y traiter
	for(int i=0 ; i<ifs.correspondances.size() ; i++) {
		if( ifs.correspondances[i].spliter > 0 ) { // Au moins une redécoupe est nécessaire
			if( dureeSplit.size() > 0 ) {
				dureeSplit.top() -= 1;
			}
			for(int k=0 ; k < ifs.correspondances[i].spliter ; k++)
				dureeSplit.push(3);
			dureeSplit.top() += 1;
		}

		int j = ifs.correspondances[i].bloc;
		decoupesEntree[ dureeSplit.size() ][j].transformer(decoupeSortie[i], ifs.correspondances[i].transformation);

		if( dureeSplit.size() > 0 ) {
			dureeSplit.top() -= 1;
		}
		while( dureeSplit.size() > 0 && dureeSplit.top() == 0) dureeSplit.pop(); // On est plus dans une partie réduite
	}
	return sortie;
}

/* *************** Enregistrement *************** */

void ImageMatricielle::sauvegarder(const char* fichier) const {
	/* Enregistre l'image au format png au nom donné
	 *  /!\ Comme on ne connais qu'une couche, l'image est enregistrée en niveaux de gris
	 */
	std::vector<unsigned char> pixels;
	for(int j=0 ; j<mHauteur ; j++) {
		for(int i=0 ; i<mLargeur ; i++) {
			pixels.push_back(mImage[i][j]);
			pixels.push_back(mImage[i][j]);
			pixels.push_back(mImage[i][j]);
			pixels.push_back(255);
		}
	}

	std::vector<unsigned char> png;
	unsigned error = lodepng::encode(png, pixels, mLargeur, mHauteur);
	if(!error) lodepng::save_file(png, fichier);

	if(error) std::cout << fichier << " -> encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

/* *************** Traitement *************** */

void ImageMatricielle::lisser(int n) {
	ImageMatricielle* copie;
	for(int k=0 ; k < n ; k++) {
		ImageMatricielle* copie = cloner();
		for(int i=1 ; i < mLargeur-1 ; i++) {
			for(int j=1 ; j < mHauteur-1 ; j++) {
				mImage[i][j] = 0;
				mImage[i][j] += (*copie)[i+1][j] / 4;
				mImage[i][j] += (*copie)[i][j+1] / 4;
				mImage[i][j] += (*copie)[i-1][j] / 4;
				mImage[i][j] += (*copie)[i][j-1] / 4;
			}
		}
		delete copie;
	}
}

void ImageMatricielle::retrecir(int reduction) {
	unsigned char **copie = mImage;

	mImage = new unsigned char* [mLargeur / reduction];
	for(int i=0 ; i < mLargeur / reduction ; i++) {
		mImage[i] = new unsigned char[ mHauteur / reduction ];
		for(int j=0 ; j < mHauteur / reduction ; j++) {
			mImage[i][j] = copie[i*reduction][j*reduction];
		}
	}

	for(int i=0 ; i < mLargeur ; i++) {
		delete[] copie[i];
	}
	delete[] copie;

	mLargeur /= reduction;
	mHauteur /= reduction;
}

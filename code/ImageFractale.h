class ImageFractale;

#ifndef IMAGE_FRACTALE
#define IMAGE_FRACTALE

#include <fstream>
#include "format.h"
#include "ImageMatricielle.h"

class ImageFractale {
	/* Décrit une image fractale
	 * Toutes les informations nécessaires au décodage sont contenuent dans la classe
	 */

	public :
		void exporter(const char* fichier); // Enregistre au format .png

		// Importe le format .png
		static ImageFractale compresser(const char* fichier, int precisionPetit, int precisionGros, bool couleur = false, bool transparence = false);

		// Enregistre l'image au format ifs
		void enregistrer(const char* fichier) const;

	protected :
		int mLargeur, mHauteur;			// Les dimensions de l'image
		bool mCouleur, mTransparence;	// L'image est en couleur ? Transparente ?
		std::vector<int> mMoyenne;		// La moyenne de teinte des couches : gris/RGB puis alpha
		std::vector<IFS> mIfs;			// Les ifs de chaque couche
};

#endif

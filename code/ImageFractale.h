class ImageFractale;

#ifndef IMAGE_FRACTALE
#define IMAGE_FRACTALE

#include <fstream>
#include <vector>
#include <cmath> // ceil

#include "debug.h"
#include "formatIfs.h"
#include "formatFichier.h"
#include "ImageMatricielle.h"


class ImageFractale {
	/* Décrit une image fractale
	 * Toutes les informations nécessaires au décodage sont contenuent dans la classe
	 */

	friend class FigureFractale;

	public :
		ImageFractale();
		// Constructeur à partir d'un fichier .ifs
		ImageFractale(const char* fichier);

		// Enregistre au format .png
		void exporter(const char* fichier);

		// Importe le format .png
		static ImageFractale compresser(const char* fichier, int precisionPetit, int precisionGros, bool couleur = false, bool transparence = false);

		// Enregistre l'image au format ifs
		void enregistrer(const char* fichier) const;

		// Crée des images schématisants la découpe
		void debugSplit() const;

		// Getters
		int getLargeur() const;
		int getHauteur() const;
		bool isCouleur() const;
		bool isTransparent() const;

	protected :
		int mLargeur, mHauteur;				// Les dimensions de l'image
		bool mCouleur, mTransparence;		// L'image est en couleur ? Transparente ?
		std::vector<unsigned char> mMoyenne;// La moyenne de teinte des couches : gris/RGB puis alpha
		std::vector<IFS> mIfs;				// Les ifs de chaque couche
};

#endif

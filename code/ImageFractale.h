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
    /* Describes a fractal image
     * All information necessary for decoding is contained in the class
     */

	friend class FigureFractale;

	public :
		ImageFractale();
		// Constructeur à partir d'un fichier .ifs  // Constructor from a .ifs file
		ImageFractale(const char* fichier);

		// Enregistre au format .png // Save as .png
		void exporter(const char* fichier);

		// Importe le format .png // Import .png
		static ImageFractale compresser(const char* fichier, int precisionPetit, int precisionGros, bool couleur = false, bool transparence = false);

		// Enregistre l'image au format ifs // Save the image as ifs
		void enregistrer(const char* fichier) const;

		// Crée une image schématisant la découpe // Create an image schematizing the cut
		void debugSplit() const;

		void grandir(int grandissement);
		void retrecir(int reduction);

		// Getters
		int getLargeur() const;
		int getHauteur() const;
		bool isCouleur() const;
		bool isTransparent() const;

	protected :
		int mLargeur, mHauteur;				// Les dimensions de l'image       // The dimensions of the image
		bool mCouleur, mTransparence;		// L'image est en couleur ? Transparente ?    // The image is in color? Transparent?
		std::vector<unsigned char> mMoyenne;// La moyenne de teinte des couches : gris/RGB puis alpha  // The average color of the layers: gray / RGB then alpha
		std::vector<IFS> mIfs;				// Les ifs de chaque couche  // The ifs of each layer
};

#endif

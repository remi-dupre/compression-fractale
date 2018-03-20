class ImagePart;

#ifndef IMAGEPART
#define IMAGEPART

#include <cmath> // pow
#include <queue>
#include "ImageMatricielle.h"
#include "formatIfs.h"

#define RAD(x) (x*3.14159265/180) // degrés -> randians

class ImagePart {
	/* Représente un bout d'image carré
	 * C'est là-dessus que sont effectuées les transformations
	 */
    /* Represents a piece of square image
      * This is where transformations are made
    */
	public :
		ImagePart(ImageMatricielle* maman, int x, int y, int taille);
		ImagePart(int taille);
		~ImagePart();


		void sauvegarder(const char* fichier) const;

		/* ********** Getters & Setters ********** */

		void set(int x, int y, unsigned char valeur);
		void remplir(unsigned char couleur);
		void encadrer();

		unsigned char at(int i, int j) const;
		int getTaille() const;

		unsigned char couleurMoyenne() const;
		float moyenneDifference( const ImagePart& partie, LinReg *decalage = NULL, bool regression = true ) const;

		/* ********** Traitement ********** */

		LinReg chercherLinReg(const ImagePart& partie) const;
		void appliquerLinReg(const LinReg& droite);

		void transformer(ImagePart& sortie, const Transformation& transfo) const;
		float chercherTransformation(const ImagePart& origine, Transformation& resultat) const;
		bool chercherMeilleur(const std::vector<ImagePart>& parties, Correspondance& meilleurCorrespondance) const;

		std::queue<ImagePart> spliter() const;

	private :
		ImageMatricielle* mImage;	// L'image dont c'est une partie  // The image of which it is a part
		int mTaille;				// La taille du coté de la partie  // The size of the side of the part
		int mX, mY;					// La position du pixel haut-gauche dans l'image source // The position of the top-left pixel in the source image
		bool mVirtuel;				// Si oui, la source a été créée uniquement pour simuler un bout d'image // If yes, the source was created only to simulate a piece of image
};

#endif

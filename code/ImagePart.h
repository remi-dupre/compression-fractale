class ImageMatricielle;

#ifndef IMAGEPART
#define IMAGEPART

#include <cmath>
#include <queue>
#include "ImageMatricielle.h"
#include "formatIfs.h"

#define RAD(x) (x*3.14159265/180) // degrés -> randians

class ImagePart {
	/* Représente un bout d'image carré
	 * C'est là-dessus que sont  effectuées les transformations
	 */

	public :
		ImagePart(ImageMatricielle* maman, int x, int y, int taille);
		ImagePart(int taille);
		~ImagePart();

		void set(int x, int y, unsigned char valeur);
		void remplir(unsigned char couleur);

		unsigned char at(int i, int j) const;
		int getTaille() const;

		unsigned char couleurMoyenne() const;
		float varianceDifference(const ImagePart& partie, LinReg *decalage = NULL, bool regression = true ) const;

		LinReg chercherLinReg(const ImagePart& partie) const;
		void appliquerLinReg(const LinReg& droite);

		void transformer(ImagePart& sortie, const Transformation& transfo) const;
		Transformation chercherTransformation(const ImagePart& origine, float& variance) const;
		Correspondance chercherMeilleur(const std::vector<ImagePart>& parties, bool *satisfaisant = NULL) const;

		std::queue<ImagePart> spliter() const;

		void sauvegarder(const char* fichier) const;

	private :
		ImageMatricielle* mImage;	// L'image dont c'est une partie
		int mTaille;				// La taille du coté de la partie
		int mX, mY;					// La position du pixel haut-gauche dans l'image source
		bool mVirtuel;				// Si oui, la source a été créée uniquement pour simuler un bout d'image
};

#endif

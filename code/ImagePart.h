#ifndef IMAGEPART
#define IMAGEPART

#include <cmath>
#include "ImageMatricielle.h"
#include "format.h"

#define RAD(x) (x*3.14159265/180) // degrés -> randians

class ImageMatricielle;
class ImagePart {
	/* Représente un bout d'image carré
	 * C'est là-dessus que sont  effectuées les transformations
	 */

	public :
		ImagePart(ImageMatricielle* maman, int x, int y, int taille);
		ImagePart(int taille);
		~ImagePart();

		void set(int x, int y, int valeur);
		void remplir(int couleur);

		int at(int i, int j) const;
		int getTaille() const;

		int couleurMoyenne() const;
		float varianceDifference(const ImagePart& partie, LinReg *decalage = NULL, bool regression = true ) const;

		LinReg chercherLinReg(const ImagePart& partie) const;
		void appliquerLinReg(const LinReg& droite);

		void transformer(ImagePart& sortie, const Transformation& transfo) const;
		Transformation chercherTransformation(const ImagePart& origine, float& variance) const; // Virer la dichotomie si elle gène ?
		Correspondance chercherMeilleur(const std::vector<ImagePart>& parties) const;

		void sauvegarder(const char* fichier) const;

	private :
		ImageMatricielle* mImage;	// L'image dont c'est une partie
		int mTaille;				// La taille du coté de la partie
		int mX, mY;					// La position du pixel haut-gauche dans l'image source
		bool mVirtuel;				// Si oui, la source a été créée uniquement pour simuler un bout d'image
};

#endif

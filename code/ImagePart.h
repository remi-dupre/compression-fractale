#ifndef imagepart
#define imagepart

#include "ImageMatricielle.h"
#include <cmath>

#define rad(x) (x*3.14159265/180)

struct Coordonnes {
	int x;
	int y;
};

struct Transformation {
	/* Décrit une transformation affine appliquée à un bloc
	 *  - Translation_x/y : le décalage de la rotation avec le centre du bloc
	 *  - Rotation : en degrés
	 */
	int rotation;
	Coordonnes translation;
};

struct Source {
	/* Décrit un couple bloc/transformation
	 *  - bloc : les coordonnées du bloc a choisir dans l'image
	 *  - transformation : le type de transformation a y appliquer
	 */
	int bloc;
	Transformation transformation;
};

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
		float varianceDifference(const ImagePart& partie) const;

		void transformer(ImagePart& sortie, const Transformation& transfo) const;
		Transformation chercherTransformation(const ImagePart& origine, float& variance) const;
		Source chercherMeilleur(const std::vector<ImagePart>& parties) const;

		void debug() const;

	private :
		ImageMatricielle* mSource;	// L'image dont c'est une partie
		int mTaille;					// La taille du coté de la partie
		int mX, mY;					// La position du pixel haut-gauche dans l'image source
		bool mVirtuel;				// Si oui, la source a été créée uniquement pour simuler un bout d'image
};

#endif

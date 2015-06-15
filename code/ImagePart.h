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
	Coordonnes bloc;
	Transformation transformation;
};

class ImageMatricielle;
class ImagePart {
	public :
		ImagePart(ImageMatricielle* maman, int x, int y, int taille);
		ImagePart(int taille);
		~ImagePart();

		void set(int x, int y, int valeur);
		void remplir(int couleur);
		
		void transformer(ImagePart& sortie, Transformation transfo); // Virer les complexes pour optimiser ?
		Transformation chercherTransformation(ImagePart origine);
		Source chercherMeilleur(ImagePart* parties);

		int at(int i, int j);
		int getTaille();
		
		int couleurMoyenne();
		float varianceDifference(ImagePart partie);
		
		void debug();

	private :
		ImageMatricielle* source; // L'image dont c'est une partie
		int taille; // La taille du coté de la partie
		int x,y;
		bool virtuel; // Si oui, la source a été créée uniquement pour simuler un bout d'image
};

#endif

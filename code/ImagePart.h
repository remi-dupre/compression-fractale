#ifndef imagepart
#define imagepart

#include "ImageMatricielle.h"
#include <cmath>

class ImageMatricielle;
class ImagePart {
	public :
		ImagePart(ImageMatricielle* maman, int x, int y, int taille);

		void set(int x, int y, int valeur);
		void remplir(int couleur);

		int at(int i, int j);
		
		int couleurMoyenne();
		float varianceDifference(ImagePart partie);
		
		void debug();

	private :
		ImageMatricielle* source; // L'image dont c'est une partie
		int taille; // La taille du coté de la partie
		int x,y;
};

#endif

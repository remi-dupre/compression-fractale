#ifndef imagepart
#define imagepart

#include "ImageMatricielle.h"
#include <cmath>
#include <complex>

#define rad(x) (x*3.14159265/180)
struct Transformation {
	int rotation; // En degrés
	double rapport;
	int translation_x,translation_y;
};

class ImageMatricielle;
class ImagePart {
	public :
		ImagePart(ImageMatricielle* maman, int x, int y, int taille);
		ImagePart(int taille);

		void set(int x, int y, int valeur);
		void remplir(int couleur);
		void transformer(ImagePart& sortie, Transformation transfo);

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

#ifndef imagematricielle
#define imagematricielle

#include <iostream>
#include <vector>
#include "ImagePart.h"

class ImagePart;
class ImageMatricielle {
	public :
		ImageMatricielle(int x, int y);
		~ImageMatricielle();

		std::vector<ImagePart>* decouper(int taille);
		
		int getLargeur();
		int getHauteur();
		
		void setLargeur(int largeur);
		void setHauteur(int hauteur);
		
		int* operator[](int i);

	private :
		int **image;
		int largeur;
		int hauteur;
};

#endif

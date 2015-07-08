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
		
		int getLargeur() const;
		int getHauteur() const;
		
		int* operator[](int i);

	private :
		int **image; // L'image est représentée par une matrice de int
		int largeur; // Largeur en pixels de l'image
		int hauteur; // Hauteur en pixels de l'image
};

#endif

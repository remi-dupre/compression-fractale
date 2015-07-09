#ifndef imagematricielle
#define imagematricielle

#include "lodepng.h"// https://github.com/lvandeve/lodepng
#include <iostream>
#include <vector>
#include "debug.h"
#include "ImagePart.h"

class ImagePart;
class ImageMatricielle {
	public :
		ImageMatricielle(const char* fichier, int couche);
		ImageMatricielle(unsigned int x, unsigned int y);
		~ImageMatricielle();

		std::vector<ImagePart>* decouper(int taille);
		void compresser(unsigned int taillePetit, unsigned int tailleGros);

		unsigned int getLargeur() const;
		unsigned int getHauteur() const;

		unsigned int* operator[](int i);

	private :
		unsigned int **image; // L'image est représentée par une matrice de int
		unsigned int largeur; // Largeur en pixels de l'image
		unsigned int hauteur; // Hauteur en pixels de l'image
};

#endif

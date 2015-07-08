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

		std::vector<ImagePart>* decouper(int taille) const;
		
		int getLargeur() const;
		int getHauteur() const;
		
		int* operator[](int i);

	private :
		int **image;
		int largeur;
		int hauteur;
};

#endif

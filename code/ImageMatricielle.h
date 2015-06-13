#include <iostream>

#ifndef imagematricielle
#define imagematricielle

class ImageMatricielle {
	public :
		ImageMatricielle(int x, int y);
		~ImageMatricielle();

		int* operator[](int i);

	private :
		int **image;
		int largeur;
		int hauteur;
};

#endif

#include "ImagePart.h"

ImagePart::ImagePart(ImageMatricielle& maman, int _x, int _y, int _taille) :
	source(maman), taille(_taille), x(_x), y(_y) {
}

void ImagePart::set(int i, int j, int val) {
	source[i+x][j+y] = val;
}

void ImagePart::remplir(int couleur) {
	for(int i=0 ; i<taille ; i++) {
		for(int j=0 ; j<taille ; j++) {
			set(i, j, couleur);
		}
	}
}

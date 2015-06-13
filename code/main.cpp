#include "ImageMatricielle.h"
#include "ImagePart.h"
#include <iostream>
#include <vector>

ImageMatricielle* rayures(int x, int y) {
	const int valeur = 255;
	const int espace = 11;
	ImageMatricielle* raye = new ImageMatricielle(x, y);
	for(int i=0 ; i<x ; i++) {
		for(int j=0 ; j<y ; j++) {
			(*raye)[i][j] = (i+j)%espace == 0 ? valeur : 0;
		}
	}
	return raye;
}

int main() {
	ImageMatricielle *img = rayures(1000, 1000);
	std::vector<ImagePart>* dan = img->decouper(10);
	int n = dan->size();
	for(int i=0 ; i<10 ; i++) {
		(*dan)[0].varianceDifference((*dan)[42]);
	}
}

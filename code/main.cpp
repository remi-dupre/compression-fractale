#include "ImageMatricielle.h"
#include "ImagePart.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

ImageMatricielle* rayures(int x, int y) {
	const int valeur = 2;
	const int espace = 4;
	ImageMatricielle* raye = new ImageMatricielle(x, y);
	for(int i=0 ; i<x ; i++) {
		for(int j=0 ; j<y ; j++) {
			(*raye)[i][j] = (i+j)%espace == 0 ? std::rand()%valeur : std::rand()%valeur;
		}
	}
	return raye;
}

int main() {
	std::srand(std::time(0));
	
	ImageMatricielle *img = rayures(1000, 1000);
	std::vector<ImagePart>* dan = img->decouper(10);
	int n = dan->size();
	
	ImagePart a(5);
	(*dan)[1337].debug();
	std::cout << "\n\n";
	
	Transformation transfo;
	transfo.translation_x = 0;
	transfo.translation_y = 0;
	transfo.rotation = 180;
	transfo.rapport = 2;
	
	(*dan)[1337].transformer(a, transfo);
	a.debug();
}

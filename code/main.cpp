#include "ImageMatricielle.h"
#include "ImagePart.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

ImageMatricielle* rayures(int x, int y) {
	const int valeur = 1;
	const int espace = 6;
	ImageMatricielle* raye = new ImageMatricielle(x, y);
	for(int i=0 ; i<x ; i++) {
		for(int j=0 ; j<y ; j++) {
			(*raye)[i][j] = (i+j)%espace == 0 ? valeur : 0;
		}
	}
	return raye;
}

ImageMatricielle* blocs(int x, int y) {
	const int valeur = 1;
	const int taille = 10;
	ImageMatricielle* sortie = new ImageMatricielle(x, y);
	for(int i=0 ; i<x ; i++) {
		for(int j=0 ; j<y ; j++) {
			(*sortie)[i][j] = (i%(taille*2) < taille) == (j%(taille*2) < taille) ? valeur : 0;
		}
	}
	return sortie;
}

int main() {
	std::srand(std::time(0));
	*img = blocs(1000, 1000);
	std::vector<ImagePart>* petit = img->decouper(30);
	std::vector<ImagePart>* gros = img->decouper(60);
	
	(*petit)[20].debug();
	for(int i=0 ; i<10 ; i++) Source a = (*petit)[20].chercherMeilleur(*gros);
	Source a = (*petit)[20].chercherMeilleur(*gros);
	ImagePart bout((*petit)[10].getTaille());
	(*gros)[a.bloc].transformer(bout, a.transformation); 
	std::cout << "\n";
	bout.debug();
}

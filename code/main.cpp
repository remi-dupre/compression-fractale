#include "ImageMatricielle.h"
#include "ImagePart.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "debug.h"
#include <fstream>

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
	ImageMatricielle image("lena.png", 0);
	std::vector<Source> ifs = image.compresser(30, 60);	

	std::cout << "Ecriture dans out.txt" << std::endl;
	std::ofstream fichier("out.txt", std::ios::trunc);
	if(fichier) {
		for(int i=0 ; i<ifs.size() ; i++) {
			fichier << i << " : " << sourceToString(ifs[i]) << std::endl;
		}
		fichier.close();
	}
	else std::cerr << "Impossible d'écrire dans le fichier" << std::endl;
}

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

ImageMatricielle randompic(int x, int y) {
	ImageMatricielle sortie(x, y);
	for(int i=0 ; i<x ; i++) {
		for(int j=0 ; j<y ; j++) {
			sortie[i][j] = rand() % 256;
		}
	}
	return sortie;
}

int main() {
	srand(time(0));

	ImageMatricielle *image = new ImageMatricielle("lena.png", 0);
	IFS ifs = image->chercherIFS(32, 42);

	std::cout << std::endl << "Ecriture dans out.txt ..." << std::endl;
	std::ofstream fichier("out.txt", std::ios::trunc);
	if(fichier) {
		for(int i=0 ; i<ifs.correspondances.size() ; i++) {
			fichier << i << "\t: " << sourceToString(ifs.correspondances[i]) << std::endl;
		}
		fichier.close();
	}
	else std::cerr << "Impossible d'écrire dans le fichier" << std::endl;

	// Décodage

	ImageMatricielle *img = new ImageMatricielle(*image);
	int moyenne = image->moyenne();
	for(int i=0 ; i<img->getLargeur() ; i++) {
		for(int j=0 ; j<img->getHauteur() ; j++) {
			(*img)[i][j] = 0; // On part du noir, ca semble mieu quand c'est lisse
		}
	}
	img->sauvegarder("depart.png");

	std::cout << "Décodage de l'image ..." << std::endl;
	for(int k=0 ; k<20 ; k++) {
		ImageMatricielle *nouveau =  new ImageMatricielle(img->appliquerIFS(ifs));
		delete img; // On désaloue pour ne pas créer de fuite de mémoire
		img = nouveau;
		std::stringstream fichier;
		fichier << "sortie/" << k << ".png";
		img->sauvegarder(fichier.str().data());
	}
	img->sauvegarder("resultat.png");
}

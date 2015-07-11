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

	ImageMatricielle *image = new ImageMatricielle("cercle.png", 2);
	IFS ifs = image->chercherIFS(12, 16);
	int moyenne = image->moyenne();

	std::cout << "Ecriture dans out.txt" << std::endl;
	std::ofstream fichier("out.txt", std::ios::trunc);
	if(fichier) {
		for(int i=0 ; i<ifs.correspondances.size() ; i++) {
			fichier << i << " : " << sourceToString(ifs.correspondances[i]) << std::endl;
		}
		fichier.close();
	}
	else std::cerr << "Impossible d'écrire dans le fichier" << std::endl;

	//ImageMatricielle *img = new ImageMatricielle( randompic(image->getLargeur(), image->getHauteur()) );
	ImageMatricielle *img = image;
	img->sauvegarder("random.png");

	std::cout << "Décodage de l'image" << std::endl;

	ImageMatricielle sorte(img->getLargeur(), img->getHauteur());
	std::vector<ImagePart> decoupeEntree = img->decouper(ifs.decoupeGros);
	std::vector<ImagePart> decoupeSortie = img->decouper(ifs.decoupePetit);

		////////////
	#define TAMER 10
	ImagePart base = decoupeSortie[TAMER];
	ImagePart sortie(base.getTaille());
	decoupeEntree[ifs.correspondances[TAMER].bloc].transformer(sortie, ifs.correspondances[TAMER].transformation);
	base.sauvegarder("_ssd.png");
	sortie.sauvegarder("_sortie.png");
	LinReg droite = base.chercherLinReg(sortie);
	std::cout << "transfo : " << droite.a << ":" << droite.b << std::endl;
	sortie.appliquerLinReg(droite);
	sortie.sauvegarder("_sortie_apres.png");
	std::cout << "moyennes : " << base.couleurMoyenne() << ":" << sortie.couleurMoyenne() << std::endl;

		////////////

	for(int k=0 ; k<10 ; k++) {
		ImageMatricielle *sortie ;
		sortie = new ImageMatricielle(img->appliquerIFS(ifs));
		//sortie->adapterMoyenne(moyenne);
		img = sortie;
		std::stringstream fichier;
		fichier << "test" << k << ".png";
		sortie->sauvegarder(fichier.str().data());
	}

	for(int k=10 ; k<40 ; k++) {
		ImageMatricielle *sortie ;
		for(int i=0;i<5;i++) {
			sortie = new ImageMatricielle(img->appliquerIFS(ifs));
			//sortie->adapterMoyenne(moyenne);
			img = sortie;
		}
		std::stringstream fichier;
		fichier << "test" << k << ".png";
		sortie->sauvegarder(fichier.str().data());
	}
}

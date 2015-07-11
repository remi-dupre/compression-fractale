#ifndef IMAGEMATRICIELLE
#define IMAGEMATRICIELLE

#include <iostream>
#include <vector>
#include <ctime>
#include "lib/lodepng.h" // https://github.com/lvandeve/lodepng
#include "ImagePart.h"
#include "format.h"
#include "debug.h"

class ImagePart;
struct Source;


typedef struct IFS IFS;
struct IFS {
	/* Décrit tout ce qu'il faut savoir sur un ifs */
	unsigned int decoupePetit;
	unsigned int decoupeGros;
	std::vector<Source> correspondances;
};

class ImageMatricielle {
	public :
		ImageMatricielle(const char* fichier, int couche);
		ImageMatricielle(unsigned int x, unsigned int y);
		~ImageMatricielle();

		std::vector<ImagePart> decouper(int taille); //todo: si c'est pas trop moche, le passer en const
		IFS chercherIFS(unsigned int taillePetit, unsigned int tailleGros);
		ImageMatricielle appliquerIFS(const IFS& ifs);

		void sauvegarder(const char* fichier) const;

		unsigned int getLargeur() const;
		unsigned int getHauteur() const;

		unsigned int moyenne() const;
		void adapterMoyenne(int val);

		unsigned int* operator[](int i);

	private :
		unsigned int **mImage; // L'image est représentée par une matrice de int
		unsigned int mLargeur; // Largeur en pixels de l'image
		unsigned int mHauteur; // Hauteur en pixels de l'image
};

#endif

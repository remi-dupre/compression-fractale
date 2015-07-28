class ImageMatricielle;

#ifndef IMAGEMATRICIELLE
#define IMAGEMATRICIELLE

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <ctime>
#include <cmath> // pow
#include <unistd.h> // sleep(int)
#include "lib/lodepng.h" // https://github.com/lvandeve/lodepng

#define timespec thread_timespec // Evite un conflict avec ctime (win)
#include <pthread.h>
#undef timespec

#include <list>

#include "ImagePart.h"
#include "formatIfs.h"
#include "debug.h"
#include "multithread.h"

class ImageMatricielle {
	public :
		ImageMatricielle(const char* fichier, int couche);
		ImageMatricielle(unsigned int x, unsigned int y);
		~ImageMatricielle();

		std::vector<ImagePart> decouper(int taille);
		static std::vector<ImagePart> adapterDecoupe(std::vector<ImagePart>&, const std::vector<Correspondance>&);

		IFS chercherIFS(int taillePetit, int tailleGros, const char* message = "");
		ImageMatricielle appliquerIFS(const IFS& ifs);

		void sauvegarder(const char* fichier) const;

		int getLargeur() const;
		int getHauteur() const;

		unsigned char moyenne() const;
		void adapterMoyenne(unsigned char val);
		void remplir(unsigned char val);

		unsigned char* operator[](int i);

	private :
		unsigned char **mImage;	// L'image est représentée par une matrice de int
		unsigned int mLargeur;	// Largeur en pixels de l'image
		unsigned int mHauteur;	// Hauteur en pixels de l'image
};

#endif

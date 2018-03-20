class ImageMatricielle;

#ifndef IMAGEMATRICIELLE
#define IMAGEMATRICIELLE

#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <ctime>
#include <cmath> // pow
#include <unistd.h> // sleep(int)
#include "lib/lodepng.h" // https://github.com/lvandeve/lodepng

#define timespec thread_timespec // Evite un conflict avec ctime (win) // Avoid a conflict with ctime (win)
#include <pthread.h>
#undef timespec


#include "ImagePart.h"
#include "formatIfs.h"
#include "multithread.h"
#include "debug.h"

class ImageMatricielle {
	public :
		ImageMatricielle(const char* fichier, int couche);
		ImageMatricielle(unsigned int x, unsigned int y);
		~ImageMatricielle();

		void sauvegarder(const char* fichier) const;
		ImageMatricielle* cloner();

		/* ********** Traitement ********** */

		std::vector<ImagePart> decouper(int taille);
		static std::vector<ImagePart> adapterDecoupe(std::vector<ImagePart>&, const std::vector<Correspondance>&);

		IFS chercherIFS(int taillePetit, int tailleGros, const char* message = "");
		ImageMatricielle appliquerIFS(const IFS& ifs);

		void lisser(int n = 1);
		void retrecir(int reduction);

		/* ********** Getters & Setters ********** */

		unsigned char* operator[](int i);
		unsigned char& at(int i, int j);

		int getLargeur() const;
		int getHauteur() const;

		unsigned char moyenne() const;
		void adapterMoyenne(unsigned char val);
		void remplir(unsigned char val);

	private :
		unsigned char **mImage;	// Les pixeles sont représentés par des octects //  Pixels are represented by octects
		unsigned int mLargeur;	// Largeur en pixels de l'image  //   Width in pixels of the image
		unsigned int mHauteur;	// Hauteur en pixels de l'image  //   Height in pixels of the image
};

#endif

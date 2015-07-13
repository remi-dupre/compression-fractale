#include "ImageFractale.h"

#include <iostream>
#include "format.h"

int main() {
	std::cout << "Flotant : " << sizeof(Flotant16b) << std::endl;
	std::cout << "En tete : " << sizeof(Pack_Entete) << std::endl;
	std::cout << "ISF : " << sizeof(Pack_IFS) << std::endl;
	std::cout << "Correspondance : " << sizeof(Pack_Correspondance) << std::endl;

	ImageFractale imgF = ImageFractale::compresser("lena.png", 32, 128, false, false);
	imgF.enregistrer("lena.ifs");

	ImageFractale imgB("lena.ifs");
	imgB.exporter("lena_resultat.png");
}

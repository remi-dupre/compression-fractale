#include "ImageFractale.h"

#include <iostream>
#include "format.h"

int main() {
	std::cout << sizeof(Transformation) << '\n';
	ImageFractale imgF = ImageFractale::compresser("lena.png", 64, 96);
	imgF.enregistrer("lena.ifs");
	imgF.exporter("lena_resultat.png");
}

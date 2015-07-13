#include "ImageFractale.h"

#include <iostream>
#include "format.h"

#include <cmath>

int main() {
	std::cout << sizeof(Transformation) << '\n';
	ImageFractale imgF = ImageFractale::compresser("lena.png", 12, 16);
	imgF.enregistrer("lena.ifs");
	imgF.exporter("lena_resultat.png");
}

#include "ImageFractale.h"

#include <iostream>
#include "debug.h"

int main() {
	ImageFractale imgF = ImageFractale::compresser("lena.png", 16, 32, false, false);
	imgF.enregistrer("lena.ifs");

	ImageFractale imgB("lena.ifs");
	imgB.exporter("lena_resultat.png");
}

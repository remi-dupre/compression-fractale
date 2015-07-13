#include "ImageFractale.h"

#include <iostream>
#include "format.h"

int main() {
	ImageFractale imgF = ImageFractale::compresser("pokemon.png", 12, 36, true, false);
	imgF.enregistrer("pokemon.ifs");

	ImageFractale imgB("pokemon.ifs");
	imgB.exporter("pokemon_resultat.png");
}
	
#include "format.h"

int main() {
	ImageFractale imgF = compresser("pokemon.png", 16, 24, true, true);
	sauvegarder("pokemon_resultat.png", imgF);
}

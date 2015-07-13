#include "ImageFractale.h"

int main() {
	ImageFractale imgF = ImageFractale::compresser("lena.png", 16, 24);
	imgF.exporter("lena_resultat.png");
}

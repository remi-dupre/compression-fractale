#include "ImageMatricielle.h"
#include "ImagePart.h"
#include <iostream>

int main() {
	ImageMatricielle img(100, 50);
	ImagePart a(img, 0, 0, 8);
	a.remplir(1337);
//	img[3][3] = 42;
	std::cout << img[3][3];
}

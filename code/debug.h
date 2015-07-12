#ifndef DEBUG
#define DEBUG

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>

#include "ImagePart.h"

#ifdef __MINGW32__
#define EFFACER() system("cls")
#endif

#ifdef __linux__
#define EFFACER() int retour_system = system("clear") ; std::cout << std::endl;
#endif

std::string chargement(int actuel, int total, int taille=30);

struct Source;
std::string sourceToString(Source source);

#endif

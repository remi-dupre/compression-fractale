#ifndef DEBUG_
#define DEBUG_

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>

#include "ImagePart.h"

#ifdef __MINGW32__
#define EFFACER() system("cls")
#elif __linux__
#define EFFACER() int retour_system = system("clear") ; std::cout << std::endl;
#endif

#define COUT extern bool SILENCIEUX ; if(!SILENCIEUX) std::cout
#define DEBUG extern bool VERBOSE ; if(VERBOSE) std::cout << "(dbg) "

std::string chargement(int actuel, int total, int taille=30);
std::string sourceToString(Correspondance source);
void debugIfs(const IFS&);

#endif

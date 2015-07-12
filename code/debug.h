#ifndef DEBUG
#define DEBUG

#include <iostream>
#include <sstream>
#include <string>

#include "ImagePart.h"

#ifdef __MINGW32__
#define EFFACER() std::system("cls")
#endif

#ifdef __LINUX__
#define EFFACER() std::system("clear")
#endif

void chargement(const char* prefixe, int actuel, int total);

struct Source;
std::string sourceToString(Source source);

#endif

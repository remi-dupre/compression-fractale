#ifndef DEBUG
#define DEBUG

#include <iostream>
#include <sstream>
#include <string>

#include "ImagePart.h"

void chargement(const char* prefixe, int actuel, int total);

struct Source;
std::string sourceToString(Source source);

#endif

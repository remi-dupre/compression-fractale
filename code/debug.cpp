#include "debug.h"

void chargement(const char* prefixe, int actuel, int total) {
	std::cout << '\r' << prefixe << " [";
	for(int i=1 ; i<=20 ; i++) {
		if( int(i*total/20) <= actuel ) std::cout << "#";
		else std::cout << "-";
	}
    std::cout << "] (" << actuel << "/" << total << ")";
}

#include "debug.h"

std::string chargement(int actuel, int total, int taille) {
	std::stringstream retour;
	retour << "[";
	for(int i=1 ; i<=taille ; i++) {
		if( int(i*total/taille) <= actuel ) retour << "#";
		else retour << "-";
	}
    retour << "] (" << actuel << "/" << total << ")";
	return retour.str();
}

std::string sourceToString(Source src) {
	std::stringstream retour;
	retour << src.bloc << " ->\t";
	retour << "rotation:" << src.transformation.rotation;
	retour << "\tdecalage:" << src.transformation.droite.a << "X+(" << src.transformation.droite.b << ")";
	return retour.str();
}

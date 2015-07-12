#include "debug.h"

std::string chargement(int actuel, int total, int taille) {
	/* Retourne une barre de chargement ascii
	 *  - actuel : l'état actuel
	 *  - total : l'état correspondant à la fin
	 *  - taille : la largeur de la barre
	 */
	std::stringstream retour;
	retour << "[";
	for(int i=1 ; i<=taille ; i++) {
		if( int(i*total/taille) <= actuel ) retour << "#";
		else retour << "-";
	}
    retour << "] (" << actuel << "/" << total << ")";
	return retour.str();
}

std::string sourceToString(Correspondance src) {
	std::stringstream retour;
	retour << src.bloc << " -> ";
	retour << "rotation:" << src.transformation.rotation;
	retour << " decalage:" << src.transformation.droite.a << "X+(" << src.transformation.droite.b << ")";
	return retour.str();
}

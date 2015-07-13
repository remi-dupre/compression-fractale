#include "formatFichier.h"

/* *************** Flotant 16 bits *************** */

float decode16bFloat(Flotant16b x) {
	/* Retourne un flotant 32 bits (natif) à partir d'un flotant 16 bits */
	return x.mantisse * std::pow(2, x.exp + DECALAGE_EXPOSANT) / 1024;
}

/* *************** Fonctions de mise en paquets *************** */

Pack_Entete packer_entete(const ImageFractale& image) {
	Pack_Entete retour;
		retour.largeur = image.getLargeur();
		retour.hauteur = image.getHauteur();
		retour.couleur = image.isCouleur();
		retour.transparence = image.isTransparent();

	if(WARNING_PACKING) {
		if(retour.largeur > 32767) std::cout << "Warning : Image trop large" << std::endl; // histoire de dire
		if(retour.hauteur > 32767) std::cout << "Warning : Image trop large" << std::endl;
	}

	return retour;
}

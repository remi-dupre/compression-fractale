#include "format.h"

unsigned char couleurLinReg(const LinReg& droite, unsigned char couleur) {
    /* Donne l'image d'une couleur par une fonction affine
     *  - droite : la fonction affine
     *  - couleur : la couleur
     */
    return std::min(255, std::max(0, int(droite.a*couleur + droite.b)));
}

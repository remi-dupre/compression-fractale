#include "format.h"

int couleurLinReg(const LinReg& droite, int couleur) {
    /* Donne l'image d'une couleur par une fonction affine
     *  - droite : la fonction affine
     *  - couleur : la couleur
     */
    return int(std::min(255, std::max(0, int(droite.a*couleur + droite.b))));
}

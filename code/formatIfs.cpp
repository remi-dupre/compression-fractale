#include "formatIfs.h"

unsigned char couleurLinReg(const LinReg& droite, unsigned char couleur) {
    /* Donne l'image d'une couleur par une fonction affine
     *  - droite : la fonction affine
     *  - couleur : la couleur
     */
    /* Give the image of a color by an affine function
      * - right: the affine function
      * - color: the color
    */
    return std::min(255, std::max(0, int(droite.a*couleur + droite.b)));
}

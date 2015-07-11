#include "format.h"

int couleurLinReg(const LinReg& droite, int couleur) {
    return int(droite.a*couleur + droite.b);
}

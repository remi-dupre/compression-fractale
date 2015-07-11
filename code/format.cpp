#include "format.h"

int couleurLinReg(const LinReg& droite, int couleur) {
    return int(std::min(255, std::max(0, int(droite.a*couleur + droite.b))));
}

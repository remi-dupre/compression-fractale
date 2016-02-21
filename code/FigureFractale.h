class FigureFractale;

#ifndef FIGURE_FRACTALE
#define FIGURE_FRACTALE

#include "ImageFractale.h"

class FigureFractale {
    public :
        static ImageFractale carre(int dimension);

        static void generer_exemples(int dimension);
};

#endif

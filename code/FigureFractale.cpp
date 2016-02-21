#include "FigureFractale.h"

Transformation zone_noire = {0, {0, 0}};
Transformation zone_blanche = {0, {0, 255}};

LinReg meme_couleur = {1, 0};

ImageFractale FigureFractale::carre(int n) {
    n /= 3;

    ImageFractale image = ImageFractale();
    image.mIfs.push_back({n, 3*n, std::vector<Correspondance>()});
    image.mMoyenne.push_back(0);
    image.mLargeur = image.mHauteur = 3*n;

    Correspondance blanc = {0, 0, zone_blanche};
    Correspondance noir = {0, 0, zone_noire};
    Correspondance copie = {0, 0, {0, {1, 0}}};
    Correspondance rot1 = {0, 0, {90, {1, 0}}};
    Correspondance rot2 = {0, 0, {360-90, {1, 0}}};


        image.mIfs[0].correspondances.push_back(copie);
        image.mIfs[0].correspondances.push_back(noir);
        image.mIfs[0].correspondances.push_back(copie);

        image.mIfs[0].correspondances.push_back(noir);
        image.mIfs[0].correspondances.push_back(blanc);
        image.mIfs[0].correspondances.push_back(noir);

        image.mIfs[0].correspondances.push_back(copie);
        image.mIfs[0].correspondances.push_back(noir);
        image.mIfs[0].correspondances.push_back(copie);

    return image;
}

/*
ImageFractale FigureFractale::feuille(int n) {
    ImageFractale image = ImageFractale();
    image.mIfs.push_back({n, 3*n, std::vector<Correspondance>()});
    image.mMoyenne.push_back(0);
    image.mLargeur = image.mHauteur = 3*n;

    Correspondance blanc = {0, 0, zone_blanche};
    Correspondance noir = {0, 0, zone_noire};

        image.mIfs[0].correspondances.push_back(noir);
        image.mIfs[0].correspondances.push_back(noir);
        image.mIfs[0].correspondances.push_back(blanc);
        image.mIfs[0].correspondances.push_back(noir);
        image.mIfs[0].correspondances.push_back(noir);
        image.mIfs[0].correspondances.push_back(noir);
        image.mIfs[0].correspondances.push_back(blanc);
        image.mIfs[0].correspondances.push_back(noir);
        image.mIfs[0].correspondances.push_back(noir);

    return image;
}
*/

void FigureFractale::generer_exemples(int n) {
    #ifdef OS_WINDOWS
    #else
    int r = system("mkdir -p examples");
    #endif

    FigureFractale::carre(n).enregistrer("examples/carre.ifs");
    FigureFractale::carre(n).exporter("examples/carre.png");
}

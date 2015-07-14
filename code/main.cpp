#include "ImageFractale.h"

#include <stdlib.h> // Pour la bibliothèque retard ci-dessous
#include <tclap/CmdLine.h>
#include <iostream>
#include "debug.h"

/*
 * Dépendances :
 *  - libtclap-dev
 *  - lpthread
 */

int main(int argc, char** argv) {
    extern bool VERBOSE;

    try {
        TCLAP::CmdLine cmd("Algorithme de compression fractal", ' ', "0.42");

        TCLAP::ValueArg<int> argTailleGros("b", "big", "La taille des gros carrés (compression)", false, 12, "int");
        TCLAP::ValueArg<int> argTaillePetit("s", "small", "La taille des petits carrés (compression)", false, 8, "int");
        TCLAP::ValueArg<std::string> argFractalFile("f", "fractal-file", "Le fichier .ifs", false, "out.ifs", "string");
        TCLAP::ValueArg<std::string> argNormalFile("p", "png-file", "Le fichier .png", false, "out.png", "string");
        TCLAP::SwitchArg argVerbose("v", "verbose", "Afficher le debugage", cmd, !VERBOSE);
        TCLAP::SwitchArg argCompresser("z", "compress", "Le fichier entré doit être compressé", cmd, false);
        TCLAP::SwitchArg argExtraire("x", "extract", "Le fichier entré doit être extrait", cmd, false);

        cmd.add( argNormalFile );
        cmd.add( argFractalFile );
        cmd.add( argTaillePetit );
        cmd.add( argTailleGros );

        cmd.parse( argc, argv );
        VERBOSE = argVerbose.getValue();
        const char* fractalFile = argFractalFile.getValue().c_str();
        const char* normalFile = argNormalFile.getValue().c_str();
        int taillePetit = argTaillePetit.getValue();
        int tailleGros = argTailleGros.getValue();

        if( argCompresser.getValue() ) { // Doit encoder
            ImageFractale imgF = ImageFractale::compresser(normalFile, taillePetit, tailleGros, false, false);
        	imgF.enregistrer( fractalFile );
        }

        if( argExtraire.getValue() ) { // Doit décoder
            ImageFractale img( fractalFile );
            img.exporter( normalFile );
        }
    }
    catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
}

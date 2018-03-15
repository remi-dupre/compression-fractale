/*
 * Programme de compression d'image par fractale (Rémi Dupré & Lucas Demarne)
 * Dépendances foireuses :
 *  - tclap (dans les dépots ubuntu, à intégrer à l'arrache avec mingw)
 *  - lpthread (nécessite d'être linké avec -lpthread)
 */

#include "FigureFractale.h"

#include "tclap/CmdLine.h"
#include <cmath> // ceil
#include "ImageFractale.h"
#include "debug.h"

#include <sstream>
#include <string>

int main(int argc, char** argv) {
	extern bool VERBOSE, SILENCIEUX;
    extern int ITERATIONS_DECOMPRESSION, NB_THREADS;
	extern int TAILLE_MIN_DECOUPE, NB_MAX_DECOUPE;

		/* *************** Lecture des entrées *************** */

	try {
		TCLAP::CmdLine cmd("Fractal compression algorithm", ' ', "0.43");

        // Paramètres de compression
		TCLAP::ValueArg<int> argTailleGros("b", "big", "The size of the big squares (compression)", false, 96, "int");
		TCLAP::ValueArg<int> argTaillePetit("s", "small", "The size of the small squares (compression)", false, 48, "int");
		TCLAP::ValueArg<int> argNbIterations("n", "nb-iterations", "The number of iterations at decompression", false, ITERATIONS_DECOMPRESSION, "int");
		TCLAP::ValueArg<int> argThreads("", "threads", "Number of maximum threads used", false, NB_THREADS, "int");
        // Fichiers d'entrée
		TCLAP::ValueArg<std::string> argFractalFile("f", "fractal-file", "The .ifs file", false, "out.ifs", "string");
		TCLAP::ValueArg<std::string> argNormalFile("p", "png-file", "The .png file", false, "out.png", "string");
        // Affichage
		TCLAP::SwitchArg argVerbose("v", "verbose", "Show debugging", cmd, !VERBOSE);
		TCLAP::SwitchArg argQuiet("q", "quiet", "Retire les affichages courants de la console", cmd, SILENCIEUX);
        // Type d'image
		TCLAP::SwitchArg argCouleur("c", "couleur", "The image must be compressed in color", cmd, false);
		TCLAP::SwitchArg argTransparence("t", "transparence", "The image must be compressed with transparency", cmd, false);
        // Type de travail
		TCLAP::SwitchArg argCompresser("z", "compress", "The file entered must be compressed", cmd, false);
		TCLAP::SwitchArg argExtraire("x", "extract", "The file entered must be extracted", cmd, false);
		TCLAP::ValueArg<int> argExamples("e", "examples", "Generate a set of type files, give their size as an argument", false, 0, "int");

		cmd.add( argNormalFile );
		cmd.add( argFractalFile );
		cmd.add( argTaillePetit );
		cmd.add( argTailleGros );
		cmd.add( argNbIterations );
		cmd.add( argThreads );
		cmd.add( argExamples );
		cmd.parse( argc, argv );

		VERBOSE = argVerbose.getValue();
		SILENCIEUX = argQuiet.getValue();
        ITERATIONS_DECOMPRESSION = argNbIterations.getValue();
        NB_THREADS = argThreads.getValue();

		const char* fractalFile = argFractalFile.getValue().c_str();
		const char* normalFile = argNormalFile.getValue().c_str();
		int taillePetit = argTaillePetit.getValue();
		int tailleGros = argTailleGros.getValue();
        bool couleur = argCouleur.getValue();
        bool transparence = argTransparence.getValue();

		TAILLE_MIN_DECOUPE = taillePetit / NB_MAX_DECOUPE;

		/* *************** Fin du traitement des entrées *************** */

		DEBUG << "Flotant : " << sizeof(Flotant16b) << "octets" << std::endl;
		DEBUG << "En tete : " << sizeof(Pack_Entete) << "octets" << std::endl;
		DEBUG << "ISF : " << sizeof(Pack_IFS) << "octets" << std::endl;
		DEBUG << "Correspondance : " << sizeof(Pack_Correspondance) << "octets" << std::endl;

		if( argCompresser.getValue() ) { // Doit encoder
			ImageFractale imgF = ImageFractale::compresser(normalFile, taillePetit, tailleGros, couleur, transparence);
			imgF.enregistrer(fractalFile);
			imgF.debugSplit();
			imgF.exporter("debug.png");
		}

		if( argExtraire.getValue() ) { // Doit décoder
			ImageFractale img( fractalFile );
			img.debugSplit();
			img.exporter( normalFile );
		}

		if( argExamples.getValue() > 0 ) { // Génération de fichiers types
			FigureFractale::generer_exemples(argExamples.getValue());
		}
	}
	catch (TCLAP::ArgException &e) {
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
}

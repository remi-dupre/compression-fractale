/*
 * Programme de compression d'image par fractale (Rémi Dupré & Lucas Demarne)
 * Dépendances foireuses :
 *  - tclap (dans les dépots ubuntu, à intégrer à l'arrache avec mingw)
 *  - lpthread (nécessite d'être linké avec -lpthread)
 */

#include <tclap/CmdLine.h>
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
		TCLAP::CmdLine cmd("Algorithme de compression fractal", ' ', "0.42");

        // Paramètres de compression
		TCLAP::ValueArg<int> argTailleGros("b", "big", "La taille des gros carrés (compression)", false, 96, "int");
		TCLAP::ValueArg<int> argTaillePetit("s", "small", "La taille des petits carrés (compression)", false, 48, "int");
		TCLAP::ValueArg<int> argNbIterations("n", "nb-iterations", "Le nombre d'itérations à la décompression", false, ITERATIONS_DECOMPRESSION, "int");
		TCLAP::ValueArg<int> argThreads("", "threads", "Nombre de threads maximum utilisés", false, NB_THREADS, "int");
        // Fichiers d'entrée
		TCLAP::ValueArg<std::string> argFractalFile("f", "fractal-file", "Le fichier .ifs", false, "out.ifs", "string");
		TCLAP::ValueArg<std::string> argNormalFile("p", "png-file", "Le fichier .png", false, "out.png", "string");
        // Affichage
		TCLAP::SwitchArg argVerbose("v", "verbose", "Afficher le debugage", cmd, !VERBOSE);
		TCLAP::SwitchArg argQuiet("q", "quiet", "Retire les affichages courants de la console", cmd, SILENCIEUX);
        // Type d'image
		TCLAP::SwitchArg argCouleur("c", "couleur", "L'image doit être compressée en couleur", cmd, false);
		TCLAP::SwitchArg argTransparence("t", "transparence", "L'image doit être compressée avec transparence", cmd, false);
        // Type de travail
		TCLAP::SwitchArg argCompresser("z", "compress", "Le fichier entré doit être compressé", cmd, false);
		TCLAP::SwitchArg argExtraire("x", "extract", "Le fichier entré doit être extrait", cmd, false);

		cmd.add( argNormalFile );
		cmd.add( argFractalFile );
		cmd.add( argTaillePetit );
		cmd.add( argTailleGros );
		cmd.add( argNbIterations );
		cmd.add( argThreads );
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
			extern float SEUIL_DECOUPE;
			//for(SEUIL_DECOUPE = 49 ; SEUIL_DECOUPE > 0 ; SEUIL_DECOUPE -= 2) {
				ImageFractale imgF = ImageFractale::compresser(normalFile, taillePetit, tailleGros, couleur, transparence);
				std::stringstream nomf;
				nomf << "out-" << SEUIL_DECOUPE << ".ifs";
				imgF.enregistrer( nomf.str().c_str() );
				std::stringstream nomp;
				nomp << "debug-" << SEUIL_DECOUPE << ".png";
				imgF.exporter(nomp.str().c_str());
			//}
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

/*
 * Les fonctions qui sont utilisées pour découper le travail en plusieurs threads
*/
/*
 * The functions that are used to split the job into multiple threads
*/

struct ThreadData;

#ifndef MULTITHREAD
#define MULTITHREAD

#include "formatIfs.h"
#include "ImagePart.h"
#include <queue>

typedef struct ThreadData ThreadData;
struct ThreadData { //TODO: Passer par des références  // Go through references
	/* Les infos qui sont utilisée par le thread   // The info that is used by the thread
	*/
	int thread_id;						// Un identifiant unique // A unique identifier
	std::queue<ImagePart> travail;		// La liste des bout d'images pour lesquel le thread doit trouver une correspondance  // The list of bits of images for which the thread must find a match
	std::vector< std::vector<ImagePart> > antecedants; // Là où sont cherchées les correspondances, en fonction de la redécoupe // Where matches are sought, depending on the redistribution
	std::vector<Correspondance>* resultat;		// Le tableau qui contiendra les résultats  /!\ doit être envoyé vide  /// The table that will contain the results /! \ Must be sent empty
};

void *lancerThread(void *data);
std::vector<Correspondance> chercherCorrespondances(std::queue<ImagePart>& travail, const std::vector< std::vector<ImagePart> >& antecedants, int redecoupe = 0);
std::vector< std::queue<ImagePart> > decouperTache(const std::vector<ImagePart>& tache, int nombre);

#endif //MULTITHREAD

/*
 * Les fonctions qui sont utilisées pour découper le travail en plusieurs threads
 */

struct ThreadData;

#ifndef MULTITHREAD
#define MULTITHREAD

#include "formatIfs.h"
#include "ImagePart.h"
#include <queue>
// #include <cstdlib> // WINDOWS ?

typedef struct ThreadData ThreadData;
struct ThreadData {
	/* Les infos qui sont utilisée par le thread
	*/
	int thread_id;						// Un identifiant unique
	std::queue<ImagePart> travail;		// La liste des bout d'images pour lesquel le thread doit trouver une correspondance
	std::vector<ImagePart> antecedants; // Là où sont cherchées les correspondances
	std::vector<Correspondance>* resultat;		// Le tableau qui contiendra les résultats  /!\ doit être envoyé vide
};

void *lancerThread(void *data);
std::vector<Correspondance> chercherCorrespondances(std::queue<ImagePart>& travail, const std::vector<ImagePart>& antecedants);
std::vector< std::queue<ImagePart> > decouperTache(const std::vector<ImagePart>& tache, int nombre);

#endif //MULTITHREAD

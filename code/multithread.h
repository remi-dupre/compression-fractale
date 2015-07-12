#ifndef MULTITHREAD
#define MULTITHREAD

#include "format.h"
#include "ImagePart.h"

struct ImagePart;

typedef struct ThreadData ThreadData;
struct ThreadData {
	/* Les infos qui sont utilisée par le thread
	*/
	int thread_id;						// Un identifiant unique (par principe)
	std::vector<ImagePart> travail;		// La liste des bout d'images pour lesquel le thread doit trouver une correspondance
	std::vector<ImagePart> antecedants; // Là où sont cherchées les correspondances
	std::vector<Correspondance>* resultat;		// Le tableau qui contiendra les résultats  /!\ doit être envoyé vide
};

void *lancerThread(void *data);
std::vector< std::vector<ImagePart> > decouperTache(const std::vector<ImagePart>& tache, unsigned int nombre);

#endif //MULTITHREAD

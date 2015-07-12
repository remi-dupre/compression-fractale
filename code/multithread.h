#ifndef MULTITHREAD
#define MULTITHREAD

#include "format.h"
#include "ImagePart.h"

struct ImagePart;

typedef struct ThreadData ThreadData;
struct ThreadData {
	int thread_id; // Identifie le thread
    int *avancement; // Pour el chargement
    int total; // Ici aussi
	std::vector<ImagePart> travail;
	std::vector<ImagePart> correspondances;
	std::vector<Source>* resultat;
};

void *lancerThread(void *data);
std::vector< std::vector<ImagePart> > decouperTache(const std::vector<ImagePart>& tache, unsigned int nombre);

#endif

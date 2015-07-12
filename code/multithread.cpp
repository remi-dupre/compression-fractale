#include "multithread.h"
	
#include <cstdlib> // WINDOWS

void *lancerThread(void *t_data) {
    ThreadData *data = (ThreadData *) t_data;
	for(int i=0 ; i < data->travail.size() ; i++) {
		data->resultat->push_back( data->travail[i].chercherMeilleur(data->correspondances) );
        ++ *(data->avancement);
		EFFACER();
        chargement(" - Recherche des correspondances", *(data->avancement), data->total);
	}
    //std::cout << "\r\033[KProcesseur " << data->thread_id  << " : terminé" << std::endl;
    pthread_exit(NULL);
}


std::vector< std::vector<ImagePart> > decouperTache(const std::vector<ImagePart>& tache, unsigned int nombre) {
    int taille = (tache.size() / nombre) + 1; // On se fait pas chier a arrondir
    std::vector< std::vector<ImagePart> > sortie(nombre, std::vector<ImagePart>());
    for(int i=0 ; i<nombre ; i++) {
        for(int j=0 ; j < taille && j+(taille*i) < tache.size() ; j++) {
            sortie[i].push_back( tache[j+(taille*i)] );
        }
    }
    return sortie;
}

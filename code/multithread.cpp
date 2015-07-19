#include "multithread.h"

void *lancerThread(void *t_data) {
    /* Lance un nouveau thread
     * Le thread calcul les sources pour chaque élément de t_data->travail puis s'arrête
     * Entrée :
     *  *t_data : pointeur sur un élément de ThreadData
     * Sortie : dans *t_data->resultat
     */
    ThreadData *data = (ThreadData *) t_data;
	while(!data->travail.empty()) {
		data->resultat->push_back( data->travail.front().chercherMeilleur(data->antecedants) );
        data->travail.pop();
	}
    pthread_exit(NULL);
}

// std::vector< std::vector<ImagePart> > decouperTache(const std::vector<ImagePart>& tache, int nombre) {
//     /* Découpe un vector en plusieurs parties, en respectant l'ordre
//      * Entrées :
//      *  - tache : un vector
//      *  - nombre : le nombre de vectors souhaités en sortie
//      * Sortie : /!\ les tableaux en sortie ne sont pas tous nécéssairement de la même taille
//      */
//     int taille = (tache.size() / nombre) + 1; // On se fait pas chier a arrondir
//     std::vector< std::vector<ImagePart> > sortie(nombre, std::vector<ImagePart>());
//     for(int i=0 ; i<nombre ; i++) {
//         for(int j=0 ; j < taille && j+(taille*i) < tache.size() ; j++) {
//             sortie[i].push_back( tache[j+(taille*i)] );
//         }
//     }
//     return sortie;
// }


std::vector< std::queue<ImagePart> > decouperTache(const std::vector<ImagePart>& tache, int nombre) {
    /* Découpe un vector en plusieurs parties, en respectant l'ordre
     * Entrées :
     *  - tache : un vector
     *  - nombre : le nombre de vectors souhaités en sortie
     * Sortie : /!\ les tableaux en sortie ne sont pas tous nécéssairement de la même taille
     */
    int taille = (tache.size() / nombre) + 1; // On se fait pas chier a arrondir
    std::vector< std::queue<ImagePart> > sortie(nombre, std::queue<ImagePart>());
    for(int i=0 ; i<nombre ; i++) {
        for(int j=0 ; j < taille && j+(taille*i) < tache.size() ; j++) {
            sortie[i].push( tache[j+(taille*i)] );
        }
    }
    return sortie;
}

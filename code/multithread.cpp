#include "multithread.h"

void *lancerThread(void *t_data) {
    /* Lance un nouveau thread
     * Le thread calcul les sources pour chaque élément de t_data->travail puis s'arrête
     * Entrée :
     *  *t_data : pointeur sur un élément de ThreadData
     * Sortie : dans *t_data->resultat
     */
    ThreadData *data = (ThreadData *) t_data;
    *data->resultat = chercherCorrespondances(data->travail, data->antecedants);
    DEBUG << "Thread " << data->thread_id << " terminé" << std::endl;
    pthread_exit(NULL);
}

std::vector<Correspondance> chercherCorrespondances(std::queue<ImagePart>& travail, const std::vector< std::vector<ImagePart> >& antecedants, int redecoupe ) {
    /* Cherche les correspondances pour un travail donné
     * Sortie :
     *  - les correspondances sont ajoutées progressivement à 'resultat'
     *  - 'travail' est régulièrement élagué
     */
    extern int TAILLE_MIN_DECOUPE;
    std::vector<Correspondance> retour;
    Correspondance correspondanceTrouvee;
	while(!travail.empty()) {
        bool satisfaisant = travail.front().chercherMeilleur(antecedants[redecoupe], correspondanceTrouvee);
        if( satisfaisant || TAILLE_MIN_DECOUPE > travail.front().getTaille() ) {
    		retour.push_back( correspondanceTrouvee ); // On est satisfait, on conserve le resultat
        }
        else {
            std::queue<ImagePart> decoupe = travail.front().spliter(); // On récupère les nouvelles parties
            std::vector<Correspondance> insertion = chercherCorrespondances(decoupe, antecedants, redecoupe+1); // On cherche les correspondances de ces bouts
            insertion[0].spliter++; // On rappel qu'on a dût spliter une fois
            for(int i=0 ; i < insertion.size() ; i++) {
                retour.push_back( insertion[i] ); // On rajoute les nouvelles correspondances
            }
        }
        travail.pop();
	}
    return retour;
}

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

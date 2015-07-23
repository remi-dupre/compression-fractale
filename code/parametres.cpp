/* Ce fichier contient les paramètres de compression
 * Ces paramètes sont présent sous la forme de variables globales car ils sont quasi-constant,
 *  mais quand même modifiable au lancement du programme
 */

/* ****************** Gestion des ressources ****************** */

// Le nombre de processus qui sont crées pour la compression
int NB_THREADS = 10;

// Le nombre d'itérations effectuées pour décompresser une image
int ITERATIONS_DECOMPRESSION = 30;

/* ****************** Algorithme de compression ****************** */

// Limite d'acceptation pour les bouts lisses
float SEUIL_LISSAGE = 1;

// Limite d'acceptation pour les transfos en général
float SEUIL_VARIANCE = 0.1;

// Limite au dessus de laquelle on redécoupe la partie
float SEUIL_DECOUPE = 625;

// Taille minimum de redécoupe
int TAILLE_MIN_DECOUPE = 4;
// Le nombre maximal de découpes qui pouront être faites, /!\ Prends le dessus sur TAILLE_MIN_DECOUPE
int NB_MAX_DECOUPE = 2;

/* ****************** Débugage ****************** */

// Si activé, désactive les affichages courrants en console
bool SILENCIEUX = false;

// Si activé, active les débugs en console
bool VERBOSE = true;

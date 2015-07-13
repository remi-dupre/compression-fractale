/* Ce fichier contient les paramètres de compression
 * Ces paramètes sont présent sous la forme de variables globales car ils sont quasi-constant,
 * | mais quand même modifiable au lancement du programme
 */

// Le nombre de processus qui sont crées pour la compression
int NB_THREADS = 10;

// Limite d'acceptation pour les bouts lisses
double SEUIL_LISSAGE = 4;

// Limite d'acceptation pour les transfos en général
double SEUIL_VARIANCE = 1;

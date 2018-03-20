/* Ce fichier contient les paramètres de compression
 * Ces paramètes sont présent sous la forme de variables globales car ils sont quasi-constant,
 *  mais quand même modifiable au lancement du programme
 */
/* This file contains the compression settings
 * These parameters are present in the form of global variables because they are almost constant,
 * but still modifiable at the launch of the program
*/

/* ****************** Resource Management ****************** */

// Le nombre de processus qui sont crées pour la compression
// The number of processes that are created for compression
int NB_THREADS = 10;

// Le nombre d'itérations effectuées pour décompresser une image
// The number of iterations performed to decompress an image
int ITERATIONS_DECOMPRESSION = 15;

/* ****************** Algorithme de compression ****************** */

// Limite d'acceptation pour les bouts lisses
// Acceptance limit for smooth tips
float SEUIL_LISSAGE = 62.5;  // Threshold smoothing / 625 

// Limite d'acceptation pour les transfos en général
// Acceptance limit for processors in general
float SEUIL_VARIANCE = 62.5;  // Threshold Variance

// Limite au dessus de laquelle on redécoupe la partie
// Limit above which the part is overlapped
float SEUIL_DECOUPE = 2000;

// Taille minimum de redécoupe
// Minimum size of overlap
int TAILLE_MIN_DECOUPE = 4;

// Le nombre maximal de découpes qui pouront être faites, /!\ Prends le dessus sur TAILLE_MIN_DECOUPE
// The maximum number of cuts that can be made, /! \ Take over on SIZE_MIN_DECOUPE
int NB_MAX_DECOUPE = 4;

/* ****************** Décompression ****************** */

int QUALITE_DECOMPRESSION = 5 ; // Ameiloration of the decompression, 1: nothing is changed // How much better is the decompression.

/* ****************** Débugage ****************** */

// Dossier contenant les resultats du debugage
// Folder containing the results of the debugging

const char* DOSSIER_DEBUG = "debug/";

// Si activé, désactive les affichages courrants en console
// If enabled, disable running console displays
bool SILENCIEUX = false;

// Si activé, active les débugs en console
// If enabled, enable console debugs
bool VERBOSE = true;

# Documentation du code

## Fichiers

### [main](https://github.com/remi100756/Compression-Fractale/blob/master/code/main.cpp)

Pas grand chose dans ce fichier à part la gestion des arguments transmis (fichier à traiter, configuration de la compression).

### [ImageFractale](https://github.com/remi100756/Compression-Fractale/blob/master/code/ImageFractale.h)

Définition de la classe ImageFractale qui décrit les caractéristiques et les données d'une image compressée par ifs. A partir d'un tel objet, on peut reconstituer l'image sans autres données, elle contient donc les méthodes de décompression (exporter), compression (importer) et d'enregistrement de l'image.

#### Attributs

- largeur / hauteur (entiers) : dimensions de l'image
- couleur / transparence (booléens) : true si l'image est en couleur / transparente
- moyenne (octect) : la couleur moyenne dans l'image
- ifs (tableau d'IFS) : les systèmes de fonctions contractantes pour chaque couche de couleur

### [formatIfs](https://github.com/remi100756/Compression-Fractale/blob/master/code/formatIfs.h)

Définition de la structure d'un IFS et d'une Transformation (fonction contractante).

### [formatFichier](https://github.com/remi100756/Compression-Fractale/blob/master/code/formatFichier.h)

Redéfinition de la structure d'IFS en optimisant l'utilisation mémoire (sûrement inutile lorsque l'on compresse), définition de l'en-tête d'un fichier.
A noter que ce fichier défini une structure de flotants 16 bits, ce qui utilise une notion du programme d'ipt de sup.

### [ImageMatricielle](https://github.com/remi100756/Compression-Fractale/blob/master/code/ImageMatricielle.h)

Défini une classe représentant une image matricielle d'une seule couche de couleur (ce qu'on sait compresser).

#### Attributs

- hauteur / largeur (pixels) : dimensions de l'image
- pixels (matrice d'entiers)

### [ImagePart](https://github.com/remi100756/Compression-Fractale/blob/master/code/ImagePart.h)

Décrit un carré dans une image, c'est sur ce type d'objet qu'on cherchera a appliquer les transformations contractante (d'un ImagePart vers un ImagePart plus petit).

#### Attributs

- image : l'image matricielle dont il est extrait
- taille (entier) : la taille du coté de l'extrait
- x, y (entiers) : la position dans l'image du pixel haut-gauche de l'extrait
- virtuel (booléen) : true si ne fait pas partie d'une vraie image

### [multithread](https://github.com/remi100756/Compression-Fractale/blob/master/code/multithread.h)

Les fonctions et les données utilisées par chaque thread lors de la recherche des correspondances.

### [parametres](https://github.com/remi100756/Compression-Fractale/blob/master/code/parametres.cpp)

Définitions de quelques constantes (globales). 

### [debug](https://github.com/remi100756/Compression-Fractale/blob/master/code/debug.h)

Quelques macros pour le debugage.

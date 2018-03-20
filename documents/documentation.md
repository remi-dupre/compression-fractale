# Code Documentation / Documentation du code

## Files  / Fichiers

### [main](https://github.com/remi100756/Compression-Fractale/blob/master/code/main.cpp)

This file (main.cpp) contains the management of the transmitted command line arguments (file to be processed, configuration of the compression).

Pas grand chose dans ce fichier à part la gestion des arguments transmis (fichier à traiter, configuration de la compression).

### [ImageFractale](https://github.com/remi100756/Compression-Fractale/blob/master/code/ImageFractale.h)

Définition de la classe ImageFractale qui décrit les caractéristiques et les données d'une image compressée par ifs. A partir d'un tel objet, on peut reconstituer l'image sans autres données, elle contient donc les méthodes de décompression (exporter), compression (importer) et d'enregistrement de l'image.

This file contains the definition of the ImageFractale class that describes the characteristics and data of an image compressed by ifs. From such an object, one can reconstruct the image without other data, it thus contains the methods of decompression (to export), compression (to import) and of recording of the image.

#### Attributess

- width / height (integers): dimensions of the image
- color / transparency (Boolean): true if the image is in color / transparent
- average (octect): the average color in the image
- ifs (IFS array): the contracting function systems for each color layer


- largeur / hauteur (entiers) : dimensions de l'image
- couleur / transparence (booléens) : true si l'image est en couleur / transparente
- moyenne (octect) : la couleur moyenne dans l'image
- ifs (tableau d'IFS) : les systèmes de fonctions contractantes pour chaque couche de couleur

### [formatIfs](https://github.com/remi100756/Compression-Fractale/blob/master/code/formatIfs.h)

Definition of the structure of an IFS and a Transformation (contracting function).

Définition de la structure d'un IFS et d'une Transformation (fonction contractante).

### [formatFichier](https://github.com/remi100756/Compression-Fractale/blob/master/code/formatFichier.h)

Redefining the structure of IFS by optimizing the memory usage (surely useless when we compress), definition of the header of a file.
Note that this file defines a 16-bit float structure, which uses a notion of the sup ipt program.

Redéfinition de la structure d'IFS en optimisant l'utilisation mémoire (sûrement inutile lorsque l'on compresse), définition de l'en-tête d'un fichier.
A noter que ce fichier défini une structure de flotants 16 bits, ce qui utilise une notion du programme d'ipt de sup.

### [ImageMatricielle](https://github.com/remi100756/Compression-Fractale/blob/master/code/ImageMatricielle.h)

Define a class representing a matrix image of a single color layer (which can be compressed).

Défini une classe représentant une image matricielle d'une seule couche de couleur (ce qu'on sait compresser).

#### Attributs

- height / width (pixels): dimensions of the image
- pixels (matrix of integers)

- hauteur / largeur (pixels) : dimensions de l'image
- pixels (matrice d'entiers)

### [ImagePart](https://github.com/remi100756/Compression-Fractale/blob/master/code/ImagePart.h)

Describes a square in an image, it is on this type of object that one will seek to apply the contracting transformations (from an ImagePart to a smaller ImagePart).

Décrit un carré dans une image, c'est sur ce type d'objet qu'on cherchera a appliquer les transformations contractante (d'un ImagePart vers un ImagePart plus petit).

#### Attributs

- image: the matrix image from which it is extracted
- size (integer): the size of the side of the extract
- x, y (integers): the position in the image of the top-left pixel of the extract
- virtual (boolean): true if not part of a real image

- image : l'image matricielle dont il est extrait
- taille (entier) : la taille du coté de l'extrait
- x, y (entiers) : la position dans l'image du pixel haut-gauche de l'extrait
- virtuel (booléen) : true si ne fait pas partie d'une vraie image

### [multithread](https://github.com/remi100756/Compression-Fractale/blob/master/code/multithread.h)

The functions and data used by each thread when searching for matches.

Les fonctions et les données utilisées par chaque thread lors de la recherche des correspondances.

### [parametres](https://github.com/remi100756/Compression-Fractale/blob/master/code/parametres.cpp)

Definitions of some (global) constants.

Définitions de quelques constantes (globales). 

### [debug](https://github.com/remi100756/Compression-Fractale/blob/master/code/debug.h)

Some macros for debugging.

Quelques macros pour le debugage.

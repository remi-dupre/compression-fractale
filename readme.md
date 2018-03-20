# fzip - Fractal Image Compression 
Algorythm to compress and decompress fractal image files in *.ifs file format.      TIPE 2015-2016.

## [Documentation]

/documents 

## Configuration

### Obtain the project source

```shell
git clone https://github.com/wrapperband/fzip.git
cd fzip
```

### Compilation

```shell
make -lpthread   # Compilation (linux)
make clean # Pour enlever les fichiers de compilation
```

### Example of running the compression algorithm to *.ifs

```shell
./fzip -p input-file.png -v -c --threads 20 -z -t -f output-file.ifs
```


###  Example of running the decompression algorithm to *.png

```shell
./fzip -p output-file.png -v -c --threads 20 -x -t -f input-file.ifs -n 2
```

### Run Time options

USAGE: 

   ./fzip  [-e <int>] [--threads <int>] [-n <int>] [-b <int>] [-s <int>]
           [-f <string>] [-p <string>] [-x] [-z] [-t] [-c] [-q] [-v] [--]
           [--version] [-h]


Where: 

   -e <int>,  --examples <int>
     Generate a set of type files, give their size as an argument ( -e 300 )

   --threads <int>
     Number of maximum threads used

   -n <int>,  --nb-iterations <int>
     The number of iterations at decompression

   -b <int>,  --big <int>
     The size of the big squares (compression)

   -s <int>,  --small <int>
     The size of the small squares (compression)

   -f <string>,  --fractal-file <string>
     The .ifs file

   -p <string>,  --png-file <string>
     The .png file

   -x,  --extract
     The file entered must be extracted

   -z,  --compress
     The file entered must be compressed

   -t,  --transparence
     The image must be compressed with transparency

   -c,  --couleur
     The image must be compressed in color

   -q,  --quiet
     Retire les affichages courants de la console

   -v,  --verbose
     Show debugging

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.


   Fractal compression algorithm


## Example

Examples are generated automatically at the following address.  (Des exemples sont générés automatiquement à l'adresse suivante)  : http://remidupre.alwaysdata.net/fractal/

![Exemple:Lena](https://raw.githubusercontent.com/remi100756/Compression-Fractale/master/lena.gif)

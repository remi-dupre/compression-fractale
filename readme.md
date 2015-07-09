# Compression Fractale
Algo de compression d'image par fractales. TIPE 2015-2016.

## Configuration

### Récupération du projet
```shell
git clone https://github.com/remi100756/Compression-Fractale.git
cd Compression-Fractale
script/configure
```

### Compilation
```shell
cd code
g++ *.cpp lib/*.cpp # Debug
g++ -s -O3 *.cpp lib/*.cpp # Release

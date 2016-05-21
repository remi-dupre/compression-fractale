[b][u]Présentation du travail[/u][/b]

[tab]J'ai réalisé ce TIPE en autonomie, cependant Lucas Demarne, un autre candidat de ma classe a traité le même sujet. Nous avons emprunté des directions différentes ce qui nous a permit de comparer nos observations, parfois très proches.

[b]Approche du sujet[/b]
[tab]Stocker une image sous un format brut entraîne des coûts en mémoire important, il est donc important de chercher à réduire la taille des informations à stocker. J'ai été attiré par par ce sujet, qui de plus me permet de redévelopper mon TPE de lycée qui portait sur le traitement d'images, mes connaissances de l'époque étant plus limitées.
[tab]Le principe de la compression d'images par fractales (1988) repose sur une idée simple : le images que nous créons représentent des objets réguliers dans lesquels différents motifs se répètent. Ce format est alors efficace pour les images comportant des surfaces mais inadapté pour des images denses en contenu, par exemple comportant du texte. En effet, ce type de compression apporte des pertes de qualité à l'image, comme par exemple le format '.jpg'.

[b]Implémentation de l'algorithme[/b]
[tab]Afin de pouvoir en étudier le comportement, j'ai moi-même développé un programme de compression d'image utilisant la méthode de compression par fractales. Le langage que j'ai décidé d'utiliser est le c++. Ce langage est compilé, ce qui permet d'atteindre des performances plus rapides qu'en utilisant l'un des deux langages de notre programme, qui sont quand à eux des langages interprétés.

[b]Compression[/b]
[tab]La création de ce programme m'a permit de tirer rapidement des conclusions sur cette méthode de compression : la création d'un fichier compressé est très lente (très coûteuse en calculs) bien que décompresser celui-ci pour le rendre à nouveau lisible ne soit très rapide, faisable en temps réel même sur les ordinateurs les plus lents.
[tab]Lucas a également créé sa version de son programme, cependant, il s'y est pris différemment sur différents aspects, la représentation de la couleur par exemple. Ma méthode de représentation de la couleur repose sur une régression linéaire (de la forme Y = [u]a[/u]X+[u]b[/u]), qui engendre un coût linéaire et demande de stocker 32 bits de données représentant [u]a[/u] et [u]b[/u]. Sa méthode de représentation est plus rudimentaire : il fixe la valeur de certains pixels, ce qui ne demande que 8 bits de données. Nous avons divergé de cette manière sur différents aspects, ce qui nous a permit de conclure sur la flexibilité de l'algorithme. Son programme est plus rapide à compresser et offre un meilleur taux de compression mais le mien et plus rapide à décompresser et dégrade moins la qualité de l'image.


[b][u]Plan[/u][/b]

La présentation de mon sujet se découpera suivant un plan de la forme suivante :

1. Algorithme de compression par fractales.
[tab]- Principe de l'algorithme
[tab]- Algorithme de compression
[tab]- Algorithme de décompression

2. Preuve de convergence
[tab]- Les systèmes de fonctions itérées
[tab]- Application du théorème du point fixe de Banach

3. Utilisation pratique : comparaison aux formats classiques d'images


[b][u]Bibliographie[/u][/b]

[tab]- [u]Topologie et analyse[/u] de Georges Skandalis
[tab]- Étude de la qualité de compression : https://karczmarczuk.users.greyc.fr/matrs/Dess/RADI/Refs/SaHaHa96a.pdf
[tab]- "Development and Comparison of Image Encoders Based on Different Compression Techniques", publié par l'École de génie des télécommunications de Barcelone
[tab]- Étude de la nasa de ce que pourrait leur apporter cette compression (date d'avant 1993) : http://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19930016738.pdf

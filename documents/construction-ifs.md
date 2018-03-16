Construction des IFS
===


## Définitions 

### Distance de Haussdorf

Soient A et B dans $$\mathcal{P}(\mathbb{C})$$, on défini la distance de Haussdorf $$\mathcal{D}$$ par $$\large{ \mathcal{D}(A, B) = max(\: \sup\limits_{a \in A} d(a, B), \sup\limits_{b \in B} d(A, b) \:) }$$

### Rotation et contraction

Pour $$(m, \theta, \alpha) \in \mathbb{C} \times \mathbb{R_+} \times \mathbb{R_+} ,$$

$$\mathcal{R}_{m, \alpha, \theta} : \begin{cases}
    \mathbb{C} & \longrightarrow & \mathbb{C} \\
    z & \longmapsto & \alpha(z-m)e^{i\theta}+m
\end{cases}$$

#### Elle est contractante
##### Pour la distance usuelle
$$\forall (a,b) \in \mathbb{C}^2 ,$$
$$\begin{array}{rl}
d(\mathcal{R}_{m, \alpha, \theta}(a), \mathcal{R}_{m, \alpha, \theta}(b) ) &
= |\ \mathcal{R}_{m, \alpha, \theta}(a) - \mathcal{R}_{m, \alpha, \theta}(b) \:| \\ &
= |\ (\alpha(a-m)e^{i\theta}+m) - (\alpha(b-m)e^{i\theta}+m) \:| \\ &
= |\ \alpha e^{i\theta} \:| . |\ (a-m) - (b-m) \:| \\ &
= \alpha |\ a-b \:| \\ &
= \alpha d(a, b)
\end{array}$$
##### Pour la distance de Haussdorf
$$\forall (A,B) \in \mathcal{P}(\mathbb{C})^2 , \forall a \in A,$$

$$\begin{array}{rl}
d(\mathcal{R}_{m, \alpha, \theta}(a), \mathcal{R}_{m, \alpha, \theta}(B) ) &
= \inf\limits_{b \in B} d(\mathcal{R}_{m, \alpha, \theta}(a), \mathcal{R}_{m, \alpha, \theta}(b) ) \\ &
= \inf\limits_{b \in B} (\: \alpha d(a, b) \:) \\ &
= \alpha \: \inf\limits_{b \in B} d(a, b) \\ &
= \alpha \mathcal{D}( \lbrace a \rbrace, B) \\ &
\end{array}$$

On en déduit donc que $$\mathcal{R}_{m, \alpha, \theta}$$ est contractante pour la distance de Haussdorf $$\mathcal{D}$$.

## Construction d'un ifs
 - Une image est représentée par un compact (fermée, bornée, etc ...) de $$\mathbb{C}$$,  $$\mathbb{I} \in \mathcal{P}(\mathbb{C})$$
 - On prend $$\alpha \in ]0,1[$$ notre rapport de contraction

### Taille d'une partie
Là je viens de me rendre compte que j'ai besoin de définir la taille d'un compact, ce qui ne doit pas être trop dur :

$$\mathcal{T} : \begin{cases}
    \mathcal{P}( \mathbb{C} ) & \longrightarrow & \mathbb{R_+} \cup \lbrace + \infty \rbrace \\
    X & \longmapsto & \sup\limits_{(a, b) \in X^2}  d(a, b)
\end{cases}$$

Et disons que l'ensemble des compacts de taille $$L \in \mathbb{R_+}$$ est :
$$\mathbb{T}_L = \lbrace X \in \mathcal{P}( \mathcal{C} ) / \mathbb{T}(X) = L \rbrace$$

### Découpe de l'image
 - $$l \in \mathbb{R_+}$$ : la largeur des gros blocs
 - $$p \in \mathbb{N^*}$$ : le nombre de petits blocs

La découpe en petits blocs de largeur $$\alpha l$$ est alors : $$(B_i)_{i \in [|1,p|]} \in (\mathbb{T}_{\alpha l} \cap \mathbb{I})^p$$ tels que :
$$\biguplus\limits_{i=1}^{p} B_i \subset \mathbb{I}$$.

Par commodité, pour $$i \in [|1,p|]$$, on pose $$m_i^B \in B_i$$. En pratique, les $$B_i$$ sont des carrés, on choisi donc leur centre.

### Création de l'ifs

#### Construction des correspondances
pour tout $$i \in [|1,p|]$$,

 - on pose $$\theta_i \in [0, 2 \pi [$$
 - on pose $$A_i \in \mathbb{T}_{l} \cap \mathbb{I}$$, un "gros bloc" et $$m_i^A \in A_i$$ le milieu de $$A_i$$
 - on pose
$$f_i : \begin{cases}
    A_i & \longrightarrow & B_i  \\
    z & \longmapsto & \mathcal{R}_{0, \alpha, \theta_i}(z-m_i^A) + m_i^B
\end{cases}$$

#### Définition de l'ifs
$$F : \begin{cases}
    \mathcal{P}( \mathbb{I} ) & \longrightarrow & \mathcal{P}( \mathbb{I} )  \\
    X & \longmapsto & \biguplus\limits_{i=1}^{p} f_i (X \cap A_i )
\end{cases}$$

## Preuve de l'existance d'un point fixe
L'application F est presque contractante, mais dans le cas général non. On doit donc prouver ça de manière détournée.

### Hypothèses
On fait l'hypotèse que $$\forall i \in [|1,p|], A_i \subset B_i$$.
Cette hypotèse ne sera en pratique pas vérifiée.

### Preuve

On pose pour $$i \in [|1,p|], g_i = F|_{\mathcal{P}( A_i )}$$.
Et donc $$g_i = \sum\limits_{A_k = A_i} f_k = \sum\limits_{B_k \subset A_i} f_k$$. Et $$A_i$$ et stable par $$g_i$$.

On pose
 - $$i \in [|1,p|]$$
 - $$(X,Y) \in \mathcal{P}(\mathbb{I})^2, x_0 \in g_i(X)$$
 - $$(x,k) \in A_i \times [|1,p|] \ tels \ que \ g_i(x) = f_k(x) = x_0$$

$$\begin{array}{rl}
d(g_i(x), g_i(Y)) &
= d(f_k(x), g_i(Y)) \\ &
= d(f_k(x), \bigcup\limits_{B_j \subset A_i} f_j(Y) ) \\ &
= \min\limits_{B_j \subset A_i} d(f_k(x), f_j(Y)) \\ &
\le d(f_k(x), f_k(Y)) \gets B_k \subset A_i \ car \ f_k(X) \cap A_i \ne \emptyset \\ &
= \alpha d(x, Y) \gets  f_k \ \text{est contractante} \\ &
\end{array}$$

On en déduit que $$g_k$$ est contractante. Le théorème du point fixe de Banach donne l'existance d'un point fixe $$X_i \subset A_i$$ de $$g_i$$. Et donc posant $$X = \bigcup\limits_{i=1}^p X_i$$ :

$$\begin{array}{rl}
F(X) &
= \biguplus\limits_{i=1}^{p} f_i (X \cap A_i) \\ &
= \biguplus\limits_{i=1}^{p} g_i (X_i) \\ &
= \biguplus\limits_{i=1}^{p} X_i \\ &
= X
\end{array}$$

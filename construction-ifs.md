Construction des IFS
===


## Définitions 

### Distance de Haussdorf

Soient A et B dans $$\mathcal{P}(\mathbb{C})$$, on défini la distance de Haussdorf $$\mathcal{D}$$ par $$\large{ \mathcal{D}(A, B) = max(\: sup_{a \in A}d(a, B), sup_{b \in B}d(A, b) \:) }$$

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
= inf_{b \in B} d(\mathcal{R}_{m, \alpha, \theta}(a), \mathcal{R}_{m, \alpha, \theta}(b) ) \\ &
= inf_{b \in B} (\: \alpha d(a, b) \:) \\ &
= \alpha \: inf_{b \in B} d(a, b) \\ &
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
    X & \longmapsto & sup_{(a, b) \in X^2}  d(a, b)
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

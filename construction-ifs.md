# Définitions

## Distance de Haussdorf

Soient A et B dans $$\mathcal{P}(\mathbb{C})$$, on définie la distance de Haussdorf $$\mathcal{D}$$ par $$\Large{ \mathcal{D}(A, B) = max(\: sup_{a \in A}d(a, B), sup_{b \in B}d(A, b) \:) }$$

## Rotation et contraction

Pour $$(m, \theta, \alpha) \in \mathbb{C} \times \mathbb{R_+} \times \mathbb{R_+} ,$$

$$\mathcal{R}_{m, \alpha, \theta} : \begin{cases}
    \mathbb{C} & \longrightarrow & \mathbb{C} \\
    z & \longmapsto & \alpha(z-m)e^{i\theta}+m
\end{cases}$$

### Elle est contractante

#### Pour la distance usuelle

$$\forall (a,b) \in \mathbb{C}^2 ,$$
$$\begin{array}{rl}
d(\mathcal{R}_{m, \alpha, \theta}(a), \mathcal{R}_{m, \alpha, \theta}(b) ) &
= |\ \mathcal{R}_{m, \alpha, \theta}(a) - \mathcal{R}_{m, \alpha, \theta}(b) \:| \\ &
= |\ (\alpha(a-m)e^{i\theta}+m) - (\alpha(b-m)e^{i\theta}+m) \:| \\ &
= |\ \alpha e^{i\theta} \:| . |\ (a-m) - (b-m) \:| \\ &
= \alpha |\ a-b \:| \\ &
= \alpha d(a, b)
\end{array}$$

#### Pour la distance de Haussdorf

$$\forall (A,B) \in \mathcal{P}(\mathbb{C})^2 , \forall a \in A,$$

$$\begin{array}{rl}
d(\mathcal{R}_{m, \alpha, \theta}(a), \mathcal{R}_{m, \alpha, \theta}(B) ) &
= inf_{b \in B} d(\mathcal{R}_{m, \alpha, \theta}(a), \mathcal{R}_{m, \alpha, \theta}(b) ) \\ &
= inf_{b \in B} (\: \alpha d(a, b) \:) \\ &
= \alpha \: inf_{b \in B} d(a, b) \\ &
= \alpha \mathcal{D}( \lbrace a \rbrace, B) \\ &
\end{array}$$

On en déduit donc que $$\mathcal{R}_{m, \alpha, \theta}$$ est contractante pour la distance de Haussdorf $$\mathcal{D}$$.

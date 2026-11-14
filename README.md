# TP de Calcul Scientifique (C/C++, Python) – MA0922

**M2 Mathématiques et Applications, parcours Calcul Scientifique (MA0922)**
Université de Reims Champagne-Ardenne

Ce dépôt regroupe les travaux pratiques réalisés dans le cadre de l'UE de Calcul Scientifique :
manipulation des arguments de la ligne de commande et des fichiers en ANSI-C90 (TP0), puis résolution de
systèmes linéaires symétriques tridiagonaux par factorisation de Cholesky (TP1).

## Sommaire

- [Structure du dépôt](#structure-du-dépôt)
- [TP0 – Passage d'arguments à un programme (`argv`)](#tp0--passage-darguments-à-un-programme-argv)
- [TP1 – Méthodes directes : matrices tridiagonales symétriques](#tp1--méthodes-directes--matrices-tridiagonales-symétriques)
- [Auteur](#auteur)

## Structure du dépôt

```
.
├── README.md
├── TP0/
│   ├── argv1.c
│   ├── argv2.c
│   ├── argv3.c
│   └── listing1.csv
└── TP1/
    ├── rapport_TP1.tex
    ├── exercice1/ex1_cholesky_tridiag.py
    ├── exercice2/ex2_structures.c
    └── exercice3/ex3_solveur.c
```

---

## TP0 – Passage d'arguments à un programme (`argv`)

Ce premier TP aborde la récupération des arguments passés en ligne de commande (`argc`, `argv`), la génération
de nombres aléatoires, la création dynamique de noms de fichiers et le parsing de fichiers CSV, en respectant la
norme **ANSI-C90**.

### Contenu du TP0

| Fichier | Description |
| :--- | :--- |
| `argv1.c` | Génère un fichier contenant $N$ valeurs réelles aléatoires dans un intervalle $[a, b]$ donné en paramètre |
| `argv2.c` | Variante générant automatiquement le nom du fichier de sortie à partir d'un préfixe et des paramètres (`Prefixe_N_min_max_.txt`) |
| `argv3.c` | Lit un fichier CSV de notes d'étudiants (`Nom,Prénom,Note`) passé en argument et affiche la moyenne et l'écart-type |

### Compilation et utilisation (TP0)

**Prérequis :** `gcc` (norme ANSI-C90 avec `-ansi -pedantic`).

```bash
cd TP0

# Exercice 1 : génération de valeurs aléatoires dans un fichier spécifié
gcc -ansi -pedantic -Wall -Wextra -o argv1 argv1.c
./argv1 100 -2.0 +2.0 fichier_sortie1.txt

# Exercice 2 : génération avec nom de fichier dynamique (ex : MonFichier_100_-5.000_+4.700_.txt)
gcc -ansi -pedantic -Wall -Wextra -o argv2 argv2.c
./argv2 100 -5.0 +4.7 MonFichier

# Exercice 3 : statistiques depuis un fichier CSV (-lm requis pour sqrt)
gcc -ansi -pedantic -Wall -Wextra -o argv3 argv3.c -lm
./argv3 listing1.csv
```

---

## TP1 – Méthodes directes : matrices tridiagonales symétriques

Ce TP porte sur la résolution de systèmes linéaires symétriques tridiagonaux $Ax = b$ par la factorisation de
Cholesky $A = C C^T$ (où $C$ est bidiagonale inférieure), en Python puis en langage C, avec un stockage mémoire
adapté à la structure de la matrice.

### Contenu du TP1

| Fichier (dans `TP1/`) | Description |
| :--- | :--- |
| `exercice1/ex1_cholesky_tridiag.py` | Factorisation, descente, remontée et solveur `solveur_chol_tridiag(A, b)` en Python, avec validation et mesure de temps |
| `exercice2/ex2_structures.c` | Structures C `Vecteur` et `BiDiag`, avec leurs fonctions d'allocation et de désallocation |
| `exercice3/ex3_solveur.c` | Solveur complet en C, en un seul fichier, la dimension $n$ étant passée en argument |
| `rapport_TP1.tex` | Rapport LaTeX : formules, algorithmes, calcul détaillé de la complexité, résultats |

### Principe

En identifiant $A = C C^T$, les coefficients de $C$ se calculent par :

$$c_{1,1} = \sqrt{a_{1,1}}$$

$$c_{i,i-1} = \frac{a_{i,i-1}}{c_{i-1,i-1}}, \qquad c_{i,i} = \sqrt{a_{i,i} - c_{i,i-1}^2} \qquad \text{pour } i = 2, \dots, n$$

Puis on résout $C y = b$ (descente) et $C^T x = y$ (remontée).

### Complexité

| Étape | Nombre d'opérations |
| :--- | :--- |
| Factorisation $A = C C^T$ | $4n - 3$ |
| Descente $C y = b$ | $3n - 2$ |
| Remontée $C^T x = y$ | $3n - 2$ |
| **Solveur complet** | $10n - 7 = \Theta(n)$ |

À comparer à $\frac{n^3}{3}$ pour Cholesky d'une matrice pleine. Le détail du calcul figure dans le rapport.

### Stockage

$A$ et $C$ sont stockées par leurs deux seules diagonales utiles ($2n - 1$ réels au lieu de $n^2$) :

```c
typedef struct {
    int     n;
    double *diag;    /* M(i,i),   i = 0..n-1 */
    double *sdiag;   /* M(i+1,i), i = 0..n-2 */
} BiDiag;
```

### Utilisation (TP1)

Toutes les commandes se lancent depuis le dossier `TP1/` :

```bash
cd TP1
```

#### Python (exercice 1)

**Prérequis :** Python 3 et NumPy.

```bash
python3 exercice1/ex1_cholesky_tridiag.py
```

#### C (exercices 2 et 3)

**Prérequis :** `gcc`.

```bash
# Exercice 2 : test des structures
gcc -Wall -Wextra -o ex2 exercice2/ex2_structures.c
./ex2

# Exercice 3 : solveur (n en argument)
gcc -Wall -Wextra -O2 -o ex3 exercice3/ex3_solveur.c -lm
./ex3 1000000
```

#### Rapport

```bash
pdflatex rapport_TP1.tex
pdflatex rapport_TP1.tex   # deuxième passage pour les références
```

### Validation

Système de test : $\text{tridiag}(-1, 2, -1)\, x = (1, 0, \dots, 0, 1)^T$, dont la solution exacte est
$x = (1, \dots, 1)^T$.

* Résidu $\lVert Ax - b \rVert_\infty$ de l'ordre de $10^{-16}$.
* L'erreur sur $x$ croît avec $n$ (environ $10^{-6}$ pour $n = 10^6$), car $\text{cond}(A) \sim n^2$.
* Les temps de calcul doublent quand $n$ double, ce qui confirme la complexité linéaire.

---

## Auteur

**ADDA Josaphat** – M2 Calcul Scientifique, Université de Reims Champagne-Ardenne
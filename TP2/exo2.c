#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double **alloc_dtab_2d(int n, int m) {
    double **tab = malloc(n * sizeof(double *));
    if (tab == NULL) {
        return 0;
    }
    
    for (int i = 0; i < n; i++) {
        tab[i] = malloc(m * sizeof(double));
    }
    return tab;
}

double **free_dtab_2d(double **tab) {
    for (int k = 0; k < sizeof(tab) / sizeof(tab[0]); k++) {
        free(tab[k]);
    }
    free(tab);
    return NULL;
}

typedef struct {
    int n; // nombre de lignes
    int m; // nombre de colonnes
    double **tab; // tableau 2D de doubles
} Matrice;

Matrice *alloc_matrice(int n, int m) {
    Matrice *A = malloc(sizeof(Matrice));
    if (A == NULL) {
        return 0;
    }
    A->n = n;
    A->m = m;
    A->tab = alloc_dtab_2d(n, m);
    return A;
}

Matrice *free_matrice(Matrice *A) {
    free_dtab_2d(A->tab);
    free(A);
    return NULL;
}

void set_matrice(double valeur, Matrice *A , int n, int m) {
    for (int i = 0; i < A->n; i++) {
        for (int j = 0; j < A->m; j++) {
            A->tab[i][j] = valeur;
        }
    }
}
double get_matrice(Matrice *A, int i, int j) {
    if (i < 0 || i >= A->n || j < 0 || j >= A->m) {
        return 1; // Indices hors limites, on quitte le programme avec un code d'erreur
    }
    return A->tab[i][j];
}
void display_matrice(char *message, Matrice *A) {
    printf("%s\n", message);
    for (int i = 0; i < A->n; i++) {
        for (int j = 0; j < A->m; j++) {
            printf("%f ", A->tab[i][j]);
        }
        printf("\n");
    }
}


int cholesky(Matrice *A, Matrice *L) {
    for (int k = 0; k < A->n; k++) {
        if (A->tab[k][k] <= 0) {
            return 1; // La matrice n'est pas définie positive
        }
        L->tab[k][k] = sqrt(A->tab[k][k]);

        // 1. D'abord calculer TOUTE la colonne k de L
        for (int i = k + 1; i < A->n; i++) {
            L->tab[i][k] = A->tab[i][k] / L->tab[k][k];
        }

        // 2. Ensuite mettre à jour la sous-matrice restante dans A
        for (int i = k + 1; i < A->n; i++) {
            for (int j = k + 1; j <= i; j++) {
                A->tab[i][j] -= L->tab[i][k] * L->tab[j][k];
            }
        printf("x[%d][%d] = %f\n", i, k, L->tab[i][k]);
        }
    }
    return 0; // Succès : L contient le facteur de Cholesky
}

int descente(Matrice *L, Matrice *x, Matrice *y) {
    
    for (int i = 0; i < L->n; i++) {
        double somme = 0.0;
        for (int j = 0; j < i; j++) {
            somme += L->tab[i][j] * y->tab[j][0];
        }
        y->tab[i][0] = (x->tab[i][0] - somme) / L->tab[i][i];
    }
    
    return 0;
}  

int remontee(Matrice *L, Matrice *y, Matrice *x) {
    for (int i = L->n - 1; i >= 0; i--) {
        double somme = 0.0;
        for (int j = i + 1; j < L->n; j++) {
            somme += L->tab[j][i] * x->tab[j][0];
        }
        x->tab[i][0] = (y->tab[i][0] - somme) / L->tab[i][i];
    }
    return 0;
}

int solveur_chol_tridiag(Matrice *A, Matrice *b, Matrice *x) {
    Matrice *C = alloc_matrice(A->n, A->m);
    Matrice *y = alloc_matrice(A->n, 1);
    if (C == NULL || y == NULL) {
        free_matrice(C);
        free_matrice(y);
        return 1;
    }

    if (cholesky(A, C) != 0) {
        free_matrice(C);
        free_matrice(y);
        return 1;
    }

    descente(C, b, y);
    remontee(C, y, x);

    free_matrice(C);
    free_matrice(y);
    return 0;
}

int main() {
    int n = 8;
    Matrice *A = alloc_matrice(n, n);
    Matrice *b = alloc_matrice(n, 1);
    Matrice *x = alloc_matrice(n, 1);

    if (A == NULL || b == NULL || x == NULL) {
        printf("Erreur d'allocation\n");
        return 1;
    }

    // Initialisation de la matrice A et du vecteur b
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                A->tab[i][j] = 2.0; // Diagonale principale
            } else if (abs(i - j) == 1) {
                A->tab[i][j] = -1.0; // Diagonales secondaires
            } else {
                A->tab[i][j] = 0.0; // Autres éléments
            }
        }
        b->tab[i][0] = 1.0; // Vecteur b
    }

    if (solveur_chol_tridiag(A, b, x) != 0) {
        printf("Echec de la factorisation\n");
        return 1;
    }

    // Affichage du résultat
    printf("Solution x:\n");
    for (int i = 0; i < n; i++) {
        printf("%f\n", x->tab[i][0]);
    }

    free_matrice(A);
    free_matrice(b);
    free_matrice(x);
    
    return 0;
}
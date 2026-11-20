#include <stdio.h>
#include <stdlib.h>

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

int main() {
    
    return 0;
}
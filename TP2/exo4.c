#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>



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

typedef struct{
    int n;
    double *tab;
} Vecteur;

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
void display_matrice(char *message, Matrice *A) {
    printf("%s\n", message);
    for (int i = 0; i < A->n; i++) {
        for (int j = 0; j < A->m; j++) {
            printf("%f ", A->tab[i][j]);
        }
        printf("\n");
    }
}

void matvec_ligne(Matrice *A, Vecteur *x, Vecteur *b) {
    for (int i = 0; i < A->n; i++) {
        b->tab[i] = 0.0;
        // interpretation de la multiplication matrice-vecteur par ligne
        for (int j = 0; j < A->m; j++) {
            b->tab[i] += A->tab[i][j] * x->tab[j];
        }
    }

    
}
void matvec_colonne(Matrice *A, Vecteur *x, Vecteur *b) {
    for (int i = 0; i < A->n; i++) {
        b->tab[i] = 0.0;
    }
    // interpretation de la multiplication matrice-vecteur par colonne
    for (int j = 0; j < A->m; j++) {
        for (int i = 0; i < A->n; i++) {
            b->tab[j] += A->tab[i][j] * x->tab[j];
        }
        
    }
}
int  main() {
    double t0, t1;
    double temps;
    int n = 3000;
    int m = 3000;
    Matrice *A = alloc_matrice(n, m);
    Vecteur *x = malloc(sizeof(Vecteur));
    x->n = m;
    x->tab = malloc(m * sizeof(double));
    Vecteur *b = malloc(sizeof(Vecteur));
    b->n = n;
    b->tab = malloc(n * sizeof(double));

    // Initialisation de la matrice A et du vecteur x
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            A->tab[i][j] = (i == j) ? 1.0 : 0.0; // Matrice identité
        }
        x->tab[i] = 1.0; // Vecteur de 1
    }
    t0 = clock();
    matvec_ligne(A, x, b);
    t1 = clock();
    temps = ((double)(t1 - t0)) / CLOCKS_PER_SEC;
    printf("Temps d'exécution (par ligne): %f secondes\n", temps);
    printf("Résultat de la multiplication matrice-vecteur (par ligne):\n");
    /*for (int i = 0; i < n; i++) {
        printf("%f\n", b->tab[i]);
    }*/
    printf("\n");
    printf("Résultat de la multiplication matrice-vecteur (par colonne):\n");
    t0 = clock();
    matvec_colonne(A, x, b);
    t1 = clock();
    temps = ((double)(t1 - t0)) / CLOCKS_PER_SEC;
    printf("Temps d'exécution (par colonne): %f secondes\n", temps);
    /*for (int i = 0; i < n; i++) {
        printf("%f\n", b->tab[i]);
    }*/
    free_matrice(A);
    free(x->tab);
    free(x);
    free(b->tab);
    free(b);

    return 0;
}
       


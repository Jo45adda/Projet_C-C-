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

void Solveur_Gauss(Matrice *A, Vecteur *b, Vecteur *x) {
    int n = A->n;
    double eps = 1e-12; // tolérance pour vérifier si un pivot est nul
    for (int k = 0; k < n - 1; k++) {
        if (fabs(A->tab[k][k]) < eps) {
            printf("A non PMP selon EPS\n");
            return;
        }
        for (int i = k + 1; i < n; i++) {
            double factor = A->tab[i][k] / A->tab[k][k];
            for (int j = k; j < n; j++) {
                A->tab[i][j] -= factor * A->tab[k][j];
            }
            b->tab[i] -= factor * b->tab[k];
        }
    }
    // Remontée
    for (int i = n - 1; i >= 0; i--) {
        x->tab[i] = b->tab[i];
        for (int j = i + 1; j < n; j++) {
            x->tab[i] -= A->tab[i][j] * x->tab[j];
        }
        x->tab[i] /= A->tab[i][i];
    }
}

int main(){
    int n, m;
    printf("Entrez le nombre de lignes (n) : ");
    scanf("%d", &n);
    printf("Entrez le nombre de colonnes (m) : ");
    scanf("%d", &m);
    Matrice *A = alloc_matrice(n, m);
    Vecteur *b = malloc(sizeof(Vecteur));
    b->n = n;
    b->tab = malloc(n * sizeof(double));
    Vecteur *x = malloc(sizeof(Vecteur));
    x->n = n;
    x->tab = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (i==j) {
                A->tab[i][j] = (double) (n + 1); // Remplissage de la diagonale principale avec n+1
            }
            else {
                A->tab[i][j] = -1.0; // Remplissage des diagonales adjacentes avec -1
            }
        }
    }
    for (int i = 0; i < n; i++) {
        b->tab[i] = 2.0; // Remplissage du vecteur b avec des 1
    }
    Solveur_Gauss(A, b, x);
    printf("Solution x :\n");
    for (int i = 0; i < n; i++) {
        printf("%f\n", x->tab[i]);
    }
    // Libération de la mémoire
    free_matrice(A);
    free(b->tab);
    free(b);
    free(x->tab);
    free(x);
}
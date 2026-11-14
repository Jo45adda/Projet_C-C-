#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int n;
    double *val;
} Vecteur;

Vecteur *alloc_Vecteur(int n) {
    if (n <= 0) return NULL;
    Vecteur *x = malloc(sizeof(Vecteur));
    if (x == NULL) return NULL;
    x->n = n;
    x->val = calloc(n, sizeof(double));
    if (x->val == NULL) {
        free(x);
        return NULL;
    }
    return x;
}

Vecteur *free_Vecteur(Vecteur *x) {
    if (x != NULL) {
        free(x->val);
        free(x);
    }
    return NULL;
}

typedef struct {
    int n;
    double *diag;
    double *sdiag;
} BiDiag;

BiDiag *alloc_BiDiag(int n) {
    if (n <= 0) return NULL;
    BiDiag *A = malloc(sizeof(BiDiag));
    if (A == NULL) return NULL;
    A->n = n;
    A->diag = calloc(n, sizeof(double));
    if (n > 1) {
        A->sdiag = calloc(n - 1, sizeof(double));
    } else {
        A->sdiag = NULL;
    }
    if (A->diag == NULL || (n > 1 && A->sdiag == NULL)) {
        free(A->diag);
        free(A->sdiag);
        free(A);
        return NULL;
    }
    return A;
}

BiDiag *free_BiDiag(BiDiag *A) {
    if (A != NULL) {
        free(A->diag);
        free(A->sdiag);
        free(A);
    }
    return NULL;
}

int main() {
    int n = 5;
    Vecteur *v = alloc_Vecteur(n);
    BiDiag *A = alloc_BiDiag(n);

    if (v == NULL || A == NULL) {
        printf("Erreur d'allocation\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        A->diag[i] = 2.0;
        v->val[i] = i + 1.0;
    }
    for (int i = 0; i < n - 1; i++) {
        A->sdiag[i] = -1.0;
    }

    printf("diag : ");
    for (int i = 0; i < n; i++) printf("%.1f ", A->diag[i]);
    printf("\nsdiag : ");
    for (int i = 0; i < n - 1; i++) printf("%.1f ", A->sdiag[i]);
    printf("\nv : ");
    for (int i = 0; i < n; i++) printf("%.1f ", v->val[i]);
    printf("\n");

    v = free_Vecteur(v);
    A = free_BiDiag(A);

    return 0;
}
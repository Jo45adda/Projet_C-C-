#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int n;
    double *val;
} Vecteur;

typedef struct {
    int n;
    double *diag;
    double *sdiag;
} BiDiag;

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

int cholesky_BiDiag(BiDiag *A, BiDiag *C) {
    int n = A->n;
    if (A->diag[0] <= 0.0) return 1;
    C->diag[0] = sqrt(A->diag[0]);

    for (int i = 1; i < n; i++) {
        C->sdiag[i - 1] = A->sdiag[i - 1] / C->diag[i - 1];
        double t = A->diag[i] - C->sdiag[i - 1] * C->sdiag[i - 1];
        if (t <= 0.0) return 1;
        C->diag[i] = sqrt(t);
    }
    return 0;
}

void descente(BiDiag *C, Vecteur *b, Vecteur *y) {
    int n = C->n;
    y->val[0] = b->val[0] / C->diag[0];
    for (int i = 1; i < n; i++) {
        y->val[i] = (b->val[i] - C->sdiag[i - 1] * y->val[i - 1]) / C->diag[i];
    }
}

void remontee(BiDiag *C, Vecteur *y, Vecteur *x) {
    int n = C->n;
    x->val[n - 1] = y->val[n - 1] / C->diag[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        x->val[i] = (y->val[i] - C->sdiag[i] * x->val[i + 1]) / C->diag[i];
    }
}

int solveur_chol_tridiag(BiDiag *A, Vecteur *b, Vecteur *x) {
    BiDiag *C = alloc_BiDiag(A->n);
    Vecteur *y = alloc_Vecteur(A->n);
    if (C == NULL || y == NULL) {
        free_BiDiag(C);
        free_Vecteur(y);
        return 1;
    }

    if (cholesky_BiDiag(A, C) != 0) {
        free_BiDiag(C);
        free_Vecteur(y);
        return 1;
    }

    descente(C, b, y);
    remontee(C, y, x);

    free_BiDiag(C);
    free_Vecteur(y);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage : %s n\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("n doit etre > 0\n");
        return 1;
    }

    BiDiag *A = alloc_BiDiag(n);
    Vecteur *b = alloc_Vecteur(n);
    Vecteur *x = alloc_Vecteur(n);
    if (A == NULL || b == NULL || x == NULL) {
        printf("Erreur d'allocation\n");
        return 1;
    }

    for (int i = 0; i < n; i++) A->diag[i] = 2.0;
    for (int i = 0; i < n - 1; i++) A->sdiag[i] = -1.0;
    b->val[0] += 1.0;
    b->val[n - 1] += 1.0;

    if (solveur_chol_tridiag(A, b, x) != 0) {
        printf("Echec de la factorisation\n");
        return 1;
    }

    double erreur = 0.0;
    double residu = 0.0;
    for (int i = 0; i < n; i++) {
        if (fabs(x->val[i] - 1.0) > erreur) erreur = fabs(x->val[i] - 1.0);
        double r = A->diag[i] * x->val[i] - b->val[i];
        if (i > 0) r += A->sdiag[i - 1] * x->val[i - 1];
        if (i < n - 1) r += A->sdiag[i] * x->val[i + 1];
        if (fabs(r) > residu) residu = fabs(r);
    }

    if (n <= 10) {
        printf("x = ");
        for (int i = 0; i < n; i++) printf("%f ", x->val[i]);
        printf("\n");
    }
    printf("n = %d\n", n);
    printf("erreur = %e\n", erreur);
    printf("residu = %e\n", residu);

    A = free_BiDiag(A);
    b = free_Vecteur(b);
    x = free_Vecteur(x);

    return 0;
}
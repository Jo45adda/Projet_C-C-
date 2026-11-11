#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main(int argc, char *argv[]) {
    int i;
    if (argc != 2) {
        printf("Usage: %s <nom_fichier> \n", argv[0]);
        return 1;
    }
    FILE *fichier = fopen(argv[1], "r");
    if (fichier == NULL) {
        perror("Error opening file");
        return 1;
    }

    char entete[100];
    if (fgets(entete, sizeof(entete), fichier) == NULL) {
        perror("Error reading header");
        fclose(fichier);
        return 1;
    }


    int nbre_eleves=0;
    double notes[100];
    char nom[50], prenom[50];
    double note;

    while (fscanf(fichier, "%s %s %lf", nom, prenom, &note) == 3) {
        notes[nbre_eleves] = note;
        nbre_eleves++;
    }


    fclose(fichier);
    
    double somme = 0.0;
    for (i = 0; i < nbre_eleves; i++) {
        somme += notes[i];
    }
    double moyenne = somme / nbre_eleves;
    printf("Moyenne des notes: %.2f\n", moyenne);
    
    double ecart_type = 0.0;
    for (i = 0; i < nbre_eleves; i++) {
        ecart_type += (notes[i] - moyenne) * (notes[i] - moyenne);
    }
    ecart_type = sqrt(ecart_type / nbre_eleves);
    printf("Ecart type des notes: %.2f\n", ecart_type);
    return 0;
}
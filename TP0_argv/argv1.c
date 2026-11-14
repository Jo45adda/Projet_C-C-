#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char *argv[]) {
    if(argc != 5) {
        printf("Usage: %s <nbr_valeur> <borne_inf> <borne_sup> <nom_fichier> \n", argv[0]);
        return 1;
    }
    int nbr_valeur = atoi(argv[1]);
    double borne_inf = atof(argv[2]);
    double borne_sup = atof(argv[3]);
    char *nom_fichier = argv[4];

    srand(time(NULL));

    FILE *fichier = fopen(nom_fichier, "w");
    if (fichier == NULL) {
        perror("Error opening file");
        return 1;
    }


    double z = borne_sup - borne_inf;
    for (int i = 0; i < nbr_valeur; i++) {
        double n = (rand() /(double)RAND_MAX)*z+ borne_inf; // Generates a random number between borne_inf and borne_sup
        fprintf(fichier, "Random number: %f\n", n);
    }

    fclose(fichier);
    return 0;
}
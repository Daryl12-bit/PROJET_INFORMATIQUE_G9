#include <stdio.h>
#include "file.h"

void dump_memory(BoiteNoire *bn){
        FILE *fichier = fopen("boite_noire_dump.bin", "wb");
        if (fichier == NULL) {
           perror("Erreur lors de louverture du fichier de dump");
            return;
        }
        Frame *temp = bn->debut;
        while (temp != NULL) {
            fwrite(temp, sizeof(Frame), 1, fichier);
            temp = temp->suivant;
        }
        fclose(fichier);
    }

    void analyse_crash(){
    FILE *fichier = fopen("boite_noire_dump.bin", "rb");
    if(!fichier){ perror("Erreur lecture crash.bin"); return; }

    Frame f;
    float somme_v = 0;
    float temp_max = 0;
    int nb = 0;

    while(fread(&f, sizeof(Frame), 1, fichier)){
        somme_v += f.vitesse;
        if(f.temperature_moteur > temp_max) temp_max = f.temperature_moteur;
        nb++;
    }

    fclose(fichier);
    printf("Analyse des donnees de la boite noire :\n");
    printf("Cause probable du crash : ");

    if(f.etat == ETAT_SURCHAUFFE){
        printf("Surchauffe du moteur\n");
    }
    else if(f.etat == ETAT_ARRET){
        printf("Vehicule a l'arret\n");
    } 
    
    if(nb>0){
        printf("Vitesse moyenne: %.2f km/h\n", somme_v / nb);
        printf("Nombre de frames analysees: %d\n", nb);
        printf("Temperature maximale: %.2f C\n", temp_max);
    }
}
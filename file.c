#include <stdio.h>
#include "file.h"

void dump_memory(BoiteNoire *bn){
        FILE *fichier = fopen("crash.bin", "wb");
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
    FILE *fichier = fopen("crash.bin", "rb");
    if(!fichier){ perror("Erreur lecture crash.bin"); return; }

    Frame f;
    float somme_v = 0;
    float temp_max = 0;
    int nb = 0;
    int a_crash_frame = 0;
    Frame last_frame;
    // Pour comportement pilote
    float vitesses[10];  // Stocke les vitesses récentes
    int idx = 0;
    int freinage_brusque = 0;
    while(fread(&f, sizeof(Frame), 1, fichier)){
        last_frame = f;
        if(f.etat == ETAT_SURCHAUFFE || f.etat == ETAT_CHOC || f.etat == ETAT_ARRET) {
            a_crash_frame = 1;
        }
        else{
        somme_v += f.vitesse;
        if(f.temperature_moteur > temp_max) temp_max = f.temperature_moteur;
        
        // Analyse comportement
        if(idx < 10) {
            vitesses[idx] = f.vitesse;
            idx++;
        }
        
        // Détection freinage brusque (chute > 30 km/h entre 2 frames)
        if(idx > 1) {
            float chute = vitesses[idx-2] - f.vitesse;
            if(chute > 30.0f) {
                freinage_brusque = 1;
            }
        } 
        nb++;
        }
    }
    
    printf("Analyse des donnees de la boite noire :\n");
    printf("Cause probable du crash : ");

    printf("\033[31m");
    if(f.etat == ETAT_SURCHAUFFE){
        printf("Surchauffe du moteur\n");
    }
    else if(f.etat == ETAT_CHOC){
        printf("Collision (freinage d'urgence)\n");
    }
    else if(f.etat == ETAT_ARRET){
        printf("Vehicule a l'arret\n");
    } 
    printf("\033[0m");
    
    if(nb>0){
        printf("\033[32m");
        printf("Vitesse moyenne: %.2f km/h\n", somme_v / nb);
        printf("Nombre de frames analysees: %d\n", nb);
        printf("Temperature maximale: %.2f C\n", temp_max);
        printf("\033[0m");
        
        // --- COMPORTEMENT PILOTE  ---
        printf("\nComportement pilote : ");
        float vitesse_moyenne = somme_v / nb;

         printf("\033[36m");
        if(vitesse_moyenne > 100.0f) {
            printf("Vitesse tres elevee maintenue\n");
        }
        else if(freinage_brusque) {
            printf("Freinages brusques detectes\n");
        }
        else if(vitesse_moyenne > 60.0f && temp_max > 90.0f) {
            printf("Conduite sportive (vitesse et temperature elevees)\n");
        }
        else {
            printf("Conduite normale\n");
        }
        printf("\033[0m");
        // --- FIN COMPORTEMENT ---
    }
    fclose(fichier);
}
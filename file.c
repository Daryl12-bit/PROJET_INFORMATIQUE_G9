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
    
    // Pour comportement pilote
    float vitesses[10];  // Stocke les vitesses récentes
    int idx = 0;
    int freinage_brusque = 0;
    float derniere_vitesse = 0;

    while(fread(&f, sizeof(Frame), 1, fichier)){
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
        
        derniere_vitesse = f.vitesse;
        nb++;
    }

    printf("Analyse des donnees de la boite noire :\n");
    printf("Cause probable du crash : ");

    if(f.etat == ETAT_SURCHAUFFE){
        printf("\033[31mSurchauffe du moteur\n");
    }
    else if(f.etat == ETAT_CHOC){
        printf("Collision (freinage d'urgence)\n");
    }
    else if(f.etat == ETAT_ARRET){
        printf("Vehicule a l'arret\033[0m\n");
    } 
    
    if(nb>0){
        printf("\033[32mVitesse moyenne: %.2f km/h\n", somme_v / nb);
        printf("Nombre de frames analysees: %d\n", nb);
        printf("Temperature maximale: %.2f C\n", temp_max);
        
        // --- COMPORTEMENT PILOTE (3 lignes) ---
        printf("\nComportement pilote : ");
        float vitesse_moyenne = somme_v / nb;
        
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
            printf("Conduite normale\033[0m\n");
        }
        // --- FIN COMPORTEMENT ---
    }
    fclose(fichier);
}
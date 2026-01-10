#include <stdio.h>
#include <stdlib.h>
#include "biblio.h"

void initialiser_frame(Frame *f, long timestamp, float vitesse, float temperature_moteur) {
    f->timestamp = timestamp;
    f->vitesse = vitesse;
    f->temperature_moteur = temperature_moteur;
    f->suivant = NULL;

}
void ajouter_frame(BoiteNoire *bn, Frame *f) {
    Frame *nouvelle_frame = (Frame *)malloc(sizeof(Frame));
    if (nouvelle_frame == NULL) {
        fprintf(stderr, "Erreur dallocation memoire\n");
        return;
    }
    *nouvelle_frame = *f;
    nouvelle_frame->suivant = NULL;

    if (bn->debut == NULL) {
        bn->debut = nouvelle_frame;
    } else {
        Frame *temp = bn->debut;
        while (temp->suivant != NULL) {
            temp = temp->suivant;
        }
        temp->suivant = nouvelle_frame;
    }
    bn->nb_frames++;
    bn->frame_actuelle++;

    if (bn->nb_frames>10){
        printf("Rotation : Frame ancienne ecrasee.\n");
        Frame *temp = bn->debut;
        bn->debut = bn->debut->suivant;
        free(temp);
        bn->nb_frames--;
    }
}
void enregistrer_cycle(BoiteNoire *bn, long timestamp, float vitesse, float temperature_moteur){
    Frame f;
    initialiser_frame(&f, timestamp, vitesse, temperature_moteur);
    ajouter_frame(bn, &f);
    
    printf("_____________________________________________________________________________\n");
    printf("Enregistrement Frame %ld (Memoire : %d/10)\n", timestamp, bn->nb_frames);
}

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
    } else if(f.etat == ETAT_ARRET){
        printf("Vehicule a l'arret\n");
    } else if(f.etat == SORTIE_UTILISATEUR){
        printf("Sortie par lutilisateur\n");}
    if(nb>0){
        printf("Vitesse moyenne: %.2f km/h\n", somme_v / nb);
        printf("Nombre de frames analysees: %d\n", nb);
        printf("Temperature maximale: %.2f C\n", temp_max);
    }
}

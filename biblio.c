#include <stdio.h>
#include <stdlib.h>
#include "biblio.h"


void initialiser_frame(Frame *f, long timestamp, float vitesse, float temperature_moteur, EtatSysteme etat, float delta_v) {
    f->timestamp = timestamp;
    f->vitesse = vitesse;
    f->temperature_moteur = temperature_moteur;
    f->acceleration= delta_v;
    f->etat = etat; // Valeur par défaut
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


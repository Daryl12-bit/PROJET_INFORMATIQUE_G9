#include <stdio.h>
#include <stdlib.h>
#include "biblio.h"
#include "simulation.h"

void setColor(const char* color) {
    printf("%s", color);
}
void enregistrer_cycle(BoiteNoire *bn, long timestamp, float vitesse, float temperature_moteur, EtatSysteme etat, float delta_v) {
    Frame f;
    initialiser_frame(&f, timestamp, vitesse, temperature_moteur ,  etat ,delta_v);
    ajouter_frame(bn, &f);

        printf("\033[32m_____________________________________________________________________________\033[0m\n");
        printf("\033[33m  Enregistrement Frame: %d s  (Memoire : %d/10)\033[0m\n", f.timestamp, bn->nb_frames);
        
    }


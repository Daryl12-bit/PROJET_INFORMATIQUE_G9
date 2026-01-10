#include <stdio.h>
#include <stdlib.h>
#include "biblio.h"
#include "stimulation.h"

void enregistrer_cycle(BoiteNoire *bn, long timestamp, float vitesse, float temperature_moteur){
    Frame f;
    initialiser_frame(&f, timestamp, vitesse, temperature_moteur);
    ajouter_frame(bn, &f);
    
    printf("_____________________________________________________________________________\n");
    printf("Enregistrement Frame %ld (Memoire : %d/10)\n", timestamp, bn->nb_frames);
}

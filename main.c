#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include "biblio.h"
#include "file.h"
#include "stimulation.h"

#define K_CHAUFFE 0.5f
#define K_COOL    0.1f
#define T_AMB     20.0f
#define TEMP_MAX  110.0f

int main(){
    BoiteNoire bn;
    Frame f;
    // maintenat on va mettre un temps en seconde qui aviendra à 10s
    bn.debut = NULL;
    bn.nb_frames = 0;
    bn.frame_actuelle = 0;
     char acceleration='A';
    float vitesse=0,temperature_moteur=0;
    char ligne[100];  
    int t=1;
    printf("Simulateur de Boite Noire\n");
    while(1){
        printf("_____________________________________________________________________________\n");
        //on va empecher l'utilisateur d'ecrire des lettres ou des phrases A PART "A" pour accelerer et "F" pour freiner
        do{
            printf("Entrez 'A' pour accelerer ou 'F' pour freiner ");
             // Lire une ligne complète
            if (fgets(ligne, sizeof(ligne), stdin) != NULL) {
                // Prendre le premier caractère et le mettre en majuscule
                acceleration = toupper(ligne[0]);
            }
        } while (acceleration != 'A' && acceleration != 'F' );

        if (acceleration == 'A') {
            vitesse+=5.0;    
            temperature_moteur = temperature_moteur + (K_CHAUFFE * vitesse) - (K_COOL * (temperature_moteur - T_AMB));

        } else if (acceleration == 'F') {

            if (vitesse<0){
                vitesse=0;}

            else if(vitesse == 0) {
                vitesse = 0;}

            else if (vitesse>=7.0){
                    vitesse-=7.0;   
                    temperature_moteur = temperature_moteur+ (K_CHAUFFE * vitesse)  - (K_COOL * (temperature_moteur - T_AMB));
                }
              
        }
        enregistrer_cycle(&bn, t, vitesse, temperature_moteur);

         printf("Ajout de la frame a %ld secondes\n", t);
        //on va afficher les infos de la frame actuelle
        Frame *temp = bn.debut;
        if (temp != NULL) {
            while (temp->suivant != NULL) { // avancer jusqu'à la dernière frame
                temp = temp->suivant;
            }
            printf("Derniere frame : Timestamp: %ld s, Vitesse: %.2f km/h, Temperature Moteur: %.2f C\n",
                temp->timestamp, temp->vitesse, temp->temperature_moteur);
        }

        if (temperature_moteur >= TEMP_MAX) {
            printf("SURCHAUFFE ! Explosion du moteur.\n");
            f.etat = ETAT_SURCHAUFFE;
            dump_memory(&bn);
            break;
        }
        if (vitesse == 0 || (acceleration=='F' && (vitesse-7.0<0))) {
            printf("Vehicule a l'arret.\n");
            f.etat = ETAT_ARRET;
            dump_memory(&bn);
            break;
        }
        t++;
        Sleep(1000); 
    }
    printf("_____________________________________________________________________________\n");
    printf("Simulation terminee.\n");
    analyse_crash();
    return 0;
}

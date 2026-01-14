#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include "biblio.h"
#include "file.h"
#include "simulation.h"
#include <time.h>
#include "acces.h"



#define K_CHAUFFE 0.5f
#define K_COOL    0.1f
#define T_AMB     20.0f
#define TEMP_MAX  110.0f
 #define ACCEL_MAX 12.0f   // km/h/s
#define DECEL_MAX_NORMAL -30.0f  // km/h/s
#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m" 

int main(){
    //Pour les codes ANSI  qui s'affiche sur le terminal windows 
    //On fait printf("\033[31mTexte en rouge\033[0m\n");
    // Le code \033[31m change la couleur du texte en rouge
    // Le code \033[0m réinitialise la couleur du texte à la valeur par défaut
     setColor(YELLOW);
    printf("Aucun espacement  nest  autorise. Espacement => Impossibilite dacces au code \n");
    setColor(RESET);
    setColor(GREEN);
    printf("Entrez votre nom d'utilisateur :\n");
    setColor(RESET);
    char nom[50];
    fgets(nom, sizeof(nom), stdin);
    nom[strcspn(nom, "\n")] = 0; // Supprimer le saut de ligne
    setColor(GREEN);
    printf("Entrez votre cle d'acces : ");
    setColor(RESET);
    char key[50];
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0; // Supprimer le saut de ligne
    if (getAccessLevel(nom, key) != 1){
        setColor(RED);
        printf("Acces refuse. Nom d'utilisateur ou cle incorrecte.\n");
        setColor(RESET);
        return 1;
    }

    //important pour rand du bas 
    BoiteNoire bn;    
    bn.debut = NULL;
    bn.nb_frames = 0;
    bn.frame_actuelle = 0;
     char acceleration='A';
    float vitesse=0,temperature_moteur=20;
    char ligne[100];  
    int t=1;
    EtatSysteme etat;
    float vitesse_souhaite=0;
    float delta_v ;
    float vitesse_min;
    float vitesse_max;
    setColor(BLUE);
    printf("Debut du Simulateur de Boite Noire\n");
    setColor(RESET);
    while(1){
        vitesse_max=ACCEL_MAX + vitesse;
        vitesse_min=DECEL_MAX_NORMAL + vitesse;
        setColor(GREEN);
        printf("_____________________________________________________________________________\n");
        setColor(RESET);
        //on va empecher l'utilisateur d'ecrire des lettres ou des phrases A PART "A" pour accelerer et "F" pour freiner
        do {
            setColor(BLUE);
            printf("Entrez 'A' pour accelerer ou 'F' pour freiner : ");
            setColor(RESET);
            fgets(ligne, sizeof(ligne), stdin);
            
            // Vérifie que la ligne fait exactement 2 caractères [lettre]\n
            if (strlen(ligne) == 2 && (ligne[0] == 'A' || ligne[0] == 'a' || 
                                    ligne[0] == 'F' || ligne[0] == 'f')) {
                acceleration = toupper(ligne[0]);
                break;
            } else {
                setColor(RED);
                printf("ERREUR : Entrez uniquement 'A' ou 'F' !\n");
                setColor(RESET);
            }
        } while (1);

        if (acceleration == 'A') {
           
            printf("Vitesse actuelle : \033[35m%.2f km/h\033[0m\n", vitesse);
            printf("Plage realisable en 1 seconde : \033[36m%.1f a %.1f km/h\033[0m\n", vitesse, vitesse_max);
            setColor(BLUE);
            printf("Entrez la vitesse souhaitee (km/h) : ");
            setColor(RESET);

            while (scanf("%f", &vitesse_souhaite) != 1) {
                setColor(RED);
                printf("Erreur : Veuillez entrer une valeur numerique pour la vitesse.\n");
                setColor(RESET);
                while(getchar() != '\n'); // Vider le buffer d'entrée
                setColor(BLUE);
                printf("Entrez la vitesse souhaitee (km/h) : ");
                setColor(RESET);
            }
            while(getchar() != '\n');

            delta_v = vitesse_souhaite - vitesse;
            if (vitesse_souhaite < 0) {
                setColor(RED);
                printf("Erreur : La vitesse ne peut pas etre negative.\n");
                setColor(RESET);
               continue;
            }
             // Vérification 1 : Si l'utilisateur essaie d'accélérer mais entre une vitesse plus basse
            if (delta_v < 0 ) {
                setColor(RED);
                printf("Erreur : En mode Acceleration, la vitesse doit etre superieure a la vitesse actuelle.\n");
                setColor(RESET);
                // On pourrait ici forcer à réessayer
                continue;
            }
             // Vérification 2 : Limites physiques d'accélération
            if (delta_v > ACCEL_MAX) {
                setColor(RED);
                printf("Attention : Acceleration trop brutale ! Vitesse Limitee a +%.1f km/h.\n", vitesse_max);
                setColor(RESET);
                continue;
            }
            temperature_moteur = temperature_moteur + (K_CHAUFFE * vitesse) - (K_COOL * (temperature_moteur - T_AMB));
            if (temperature_moteur < T_AMB) temperature_moteur = T_AMB;

        } else if (acceleration == 'F') {

            printf("Vitesse actuelle : \033[35m%.2f km/h\033[0m\n", vitesse);
            if (vitesse_min < 0) {
                printf("Plage realisable en 1 seconde :\033[33m0 km/h a %.1f km/h \033[0m\n", vitesse);
            } else {
            printf("Plage realisable en 1 seconde : \033[33m%.1f a %.1f km/h\033[0m\n", vitesse_min, vitesse);}
            setColor(BLUE);
            printf("Entrez la vitesse souhaitee (km/h) : ");
            setColor(RESET);
            
            while (scanf("%f", &vitesse_souhaite) != 1) {
                setColor(RED);
                printf("Erreur : Veuillez entrer une valeur numerique pour la vitesse.\n");
                setColor(RESET);
                while(getchar() != '\n'); // Vider le buffer d'entrée
                setColor(BLUE);
                printf("Entrez la vitesse souhaitee (km/h) : ");
                setColor(RESET);
            }
            while(getchar() != '\n');
            delta_v = vitesse_souhaite - vitesse;

            if (vitesse_souhaite < 0) {
                setColor(RED);
                printf("Erreur : La vitesse ne peut pas etre negative.\n");
                setColor(RESET);
               continue;
            }
             // Vérification 1 : Si l'utilisateur essaie de freiner mais entre une vitesse plus haute
            if (delta_v >= 0) {
                setColor(RED);
                printf("Erreur : En mode Freinage, la vitesse doit etre inferieure a la vitesse actuelle.\n");
                setColor(RESET);
                // On pourrait ici forcer à réessayer
                continue;
            }
             // Vérification 2 : Limites physiques de décélération
            if (delta_v < DECEL_MAX_NORMAL) {
                setColor(RED);
                printf("ATTENTION : FREINAGE D'URGENCE DETECTE !\n");
                printf("CRASH ! Freinage insuffisant pour eviter l'obstacle.\n");
               setColor(RESET);
            }

            temperature_moteur = temperature_moteur + (K_CHAUFFE *  vitesse) - (K_COOL * (temperature_moteur - T_AMB));       
            if (temperature_moteur < T_AMB) temperature_moteur = T_AMB;       
        }
         // Mise à jour de la vitesse
        vitesse = vitesse_souhaite;

        if (vitesse == 0 && !(delta_v < DECEL_MAX_NORMAL)) {
            etat = ETAT_ARRET; 
            setColor(RED);
                printf("Voiture a larret !\n");
            setColor(RESET);
            enregistrer_cycle(&bn, t, vitesse, temperature_moteur,etat, delta_v);
            dump_memory(&bn);
           
        }
        else if (delta_v < DECEL_MAX_NORMAL) {
            etat = ETAT_CHOC;
            enregistrer_cycle(&bn, t, vitesse, temperature_moteur,etat,delta_v);
            dump_memory(&bn);
            
        }
        else if (temperature_moteur >= TEMP_MAX) {
            etat = ETAT_SURCHAUFFE;
            setColor(RED);
                printf("ATTENTION : Surchauffe du moteur detectee !\n");
            enregistrer_cycle(&bn, t, vitesse, temperature_moteur,etat, delta_v);
            dump_memory(&bn);
        }else{
         enregistrer_cycle(&bn, t, vitesse, temperature_moteur,etat, delta_v);}
        //on va afficher les infos de la frame actuelle
        
        Frame *temp = bn.debut;
        if (temp != NULL) {
            while (temp->suivant != NULL) { // avancer jusqu'à la dernière frame
                temp = temp->suivant;
            }
            setColor(YELLOW);
            printf("Vitesse: %.2f km/h, Temperature Moteur: %.2f C\n", temp->vitesse, temp->temperature_moteur);
            setColor(RESET);
        }
        if (vitesse == 0 || temperature_moteur >= TEMP_MAX || delta_v < DECEL_MAX_NORMAL) {
            break;
        }
        t++;
        Sleep(1000); 
    }
    setColor(GREEN);
    printf("_____________________________________________________________________________\n");
    setColor(RESET);
    setColor(BLUE);
    printf("Simulation terminee.\n");
    setColor(RESET);
    analyse_crash();
    return 0;
}

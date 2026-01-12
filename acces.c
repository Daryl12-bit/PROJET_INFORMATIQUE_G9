#include "acces.h"
#include <stdio.h>
#include <string.h>
    int getAccessLevel(const char *nom, const char *key){
        FILE*f=fopen("acces.config","r");
        if (f==NULL) {
            printf("Impossible douvrir le fichier de configuration daccès.\n");
            return -1;
        }
        char ligne [100];
        char nom_fichier[50];
        char cle[50];
        while (fgets(ligne, sizeof(ligne), f)){

            if(sscanf(ligne, "%49[^:]:%49s", nom_fichier, cle)==2){
                nom_fichier[strcspn(nom_fichier, " \n")] = 0; // Supprimer espaces et saut de ligne
                cle[strcspn(cle, " \n")] = 0; // Supprimer espaces et saut de ligne
                if (strcmp(nom, nom_fichier) == 0 &&  (strcmp(key, cle) == 0)){
                    fclose(f);
                    return 1; // Accès autorisé
                    }
            }
        }
            fclose (f);
            return 0; // Accès refusé
    }
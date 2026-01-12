#ifndef BIBLIO_H
#define BIBLIO_H

    // On debute avec les structures 
     typedef enum {
        ETAT_ARRET,
        ETAT_SURCHAUFFE,
        ETAT_CHOC
    } EtatSysteme; 

    typedef struct Frame {
       long timestamp;
       float vitesse;
       float temperature_moteur;
       float acceleration;
       EtatSysteme etat;
       struct Frame *suivant;
    } Frame;

    typedef struct BoiteNoire {
        struct Frame *debut;
        int nb_frames;
        int frame_actuelle;
    } BoiteNoire;

   

    // Fin des structures
    // Déclaration des fonctions (s'il y en a)
    void initialiser_frame(Frame *f, long timestamp, float vitesse, float temperature_moteur,EtatSysteme etat, float delta_v);
    void ajouter_frame(BoiteNoire *bn, Frame *f);
  
    

#endif
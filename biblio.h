#ifndef BIBLIO_H
#define BIBLIO_H

    // On debute avec les structures 
     typedef enum {
        ETAT_ARRET,
        ETAT_SURCHAUFFE
    } EtatSysteme; 

    typedef struct Frame {
       long timestamp;
       float vitesse;
       float temperature_moteur;
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
    void initialiser_frame(Frame *f, long timestamp, float vitesse, float temperature_moteur);
    void ajouter_frame(BoiteNoire *bn, Frame *f);
    void enregistrer_cycle(BoiteNoire *bn, long timestamp, float vitesse, float temperature_moteur);
    

#endif
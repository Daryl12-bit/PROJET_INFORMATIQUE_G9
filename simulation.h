#include "biblio.h"
#ifndef SIMULATION_H
#define SIMULATION_H
// Déclarations spécifiques à la simulation (s'il y en a)
  void enregistrer_cycle(BoiteNoire *bn, long timestamp, float vitesse, float temperature_moteur ,EtatSysteme etat ,float delta_v);
  void setColor(const char* color);
#endif
#ifndef FONCTIONS_VOITURE_H_INCLUDED
#define FONCTIONS_VOITURE_H_INCLUDED
void direction_voiture(int direction,struct Gpio* gpio);
void sens_voiture(int sens,struct Gpio* gpio);
void* deplacement(void *arg);


#endif // FONCTIONS_VOITURE_H_INCLUDED

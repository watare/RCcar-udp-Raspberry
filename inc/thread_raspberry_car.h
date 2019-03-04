#include <pthread.h>

#ifndef FONCTIONS_VOITURE_H_INCLUDED
#define FONCTIONS_VOITURE_H_INCLUDED

/*structures==========================================================*/
//structure donnant la direction de la voiture et son sens de deplacement
typedef struct DeplacementVoiture {
    //sens 0 la voiture ne bouge pas
    //sens 1 la voiture va en avant
    //sens 2 la voiture va en arriere
    int sens;

    //direction 0 la voiture va tout droit
    //direction 1 la voiture va a droite
    //direction 2 la voiture va a gauche
    int direction;
    //mutex
    pthread_mutex_t mutex;

}DeplacementVoiture;


//la structure GPIO et l objet utilise pour ecrire sur les sorties physiques du raspberry
typedef struct RaspGpio {
    //numero de gpio
    int numero;
    //valeur a ecrire sur le GPIO
    int valeur;
}RaspGpio;

/*prototypes==========================================================*/

void dirigerVoiture(int direction,struct RaspGpio* p_s_gpio);
void avancerVoiture(int sens,struct RaspGpio* gpio);
void* conduireVoiture(void* arg);

#endif // FONCTIONS_VOITURE_H_INCLUDED

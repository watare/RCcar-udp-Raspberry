#include <stdio.h>
#include <stdlib.h>
#include "raspberry_car_structures.h"
#include <pthread.h>
#include <unistd.h>


#include <pigpio.h>


#define DROITE 3
#define GAUCHE 4

#define AVANT 17
#define ARRIERE 27
// liste des fonctions utilisees dans le projet
//fonction donnant la direction de deplacement de la voiture
void direction_voiture(int direction,struct Gpio* gpio){
//Les numero et valeurs des GPIO devront etre remplace par les valeurs effectivement utilisees

    switch(direction){
        case 0:
            printf("la voiture va tout droit! \n");
            //(*gpio).numero=0;
            //(*gpio).valeur=0;
            if (gpioGetPWMdutycycle(DROITE)!=0) gpioPWM(DROITE,0);
            if (gpioGetPWMdutycycle(GAUCHE)!=0) gpioPWM(GAUCHE,0);

        break;

        case 1:
            printf("la voiture va à droite! \n");
            //(*gpio).numero=0;
            //(*gpio).valeur=1;
            if (gpioGetPWMdutycycle(GAUCHE)!=0) gpioPWM(GAUCHE,0);
            if (gpioGetPWMdutycycle(DROITE)!=255) gpioPWM(DROITE,255);


        break;


        case 2:
            printf("la voiture va à gauche! \n");
            if (gpioGetPWMdutycycle(DROITE)!=0) gpioPWM(DROITE,0);
            if (gpioGetPWMdutycycle(GAUCHE)!=255) gpioPWM(GAUCHE,255);
        break;

        default:
            printf("erreur de direction! \n");
            if (gpioGetPWMdutycycle(DROITE)!=0) gpioPWM(DROITE,0);
            if (gpioGetPWMdutycycle(GAUCHE)!=0) gpioPWM(GAUCHE,0);

    };


};

void sens_voiture(int sens,struct Gpio* gpio){
//Les numero et valeurs des GPIO devront etre remplace par les valeurs effectivement utilisees

    switch(sens){
        case 0:
            printf("la voiture n'avance pas! \n");
            if (gpioGetPWMdutycycle(AVANT)!=0) gpioPWM(AVANT,0);
            if (gpioGetPWMdutycycle(ARRIERE)!=0) gpioPWM(ARRIERE,0);
        break;

        case 1:
            printf("la voiture avance! \n");
            if (gpioGetPWMdutycycle(ARRIERE)!=0) gpioPWM(ARRIERE,0);
            if (gpioGetPWMdutycycle(AVANT)!=255) gpioPWM(AVANT,255);
        break;


        case 2:
            printf("la voiture recule! \n");
            if (gpioGetPWMdutycycle(AVANT)!=0) gpioPWM(AVANT,0);
            if (gpioGetPWMdutycycle(ARRIERE)!=255) gpioPWM(ARRIERE,255);
        break;

        default:
            printf("je ne sais pas quoi faire! \n");
            if (gpioGetPWMdutycycle(AVANT)!=0) gpioPWM(AVANT,0);
            if (gpioGetPWMdutycycle(ARRIERE)!=0) gpioPWM(ARRIERE,0);

    };


};

//combinaison de deplacement
void* deplacement(void* arg){

    DeplacementVoiture* deplacement = malloc(sizeof(DeplacementVoiture));
    deplacement = arg;
    struct Gpio* gpio = malloc(sizeof(Gpio));

    if (gpioInitialise() < 0) return -1;
    gpioPWM(3,0);
    gpioPWM(4,0);
    gpioPWM(17,0);
    gpioPWM(27,0);

    while(1){

         //on rentre dans la zone protegee
        pthread_mutex_lock(&(*deplacement).mutex);

        //recuperation des valeurs udp
        int direction;
        direction =(*deplacement).direction;
        int sens;
        sens =(*deplacement).sens;

        pthread_mutex_unlock(&(*deplacement).mutex);

        //printf("%i",direction);

        //envoi des commandes sur les sorties phyisques
        direction_voiture(direction,gpio);
        sens_voiture(sens,gpio);

        pthread_mutex_lock(&(*deplacement).mutex);
        //RAZ des commandes reçues
        (*deplacement).sens=0;
        (*deplacement).direction=0;

        pthread_mutex_unlock(&(*deplacement).mutex);

       //interrompre le thread pendand n µsecondes
       usleep(100000);

    };
    pthread_exit(NULL);
};

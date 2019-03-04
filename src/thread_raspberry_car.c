
#include "../inc/thread_raspberry_car.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//fonction donnant la direction de deplacement de la voiture
void dirigerVoiture(int direction,struct RaspGpio* p_s_gpio){
//Les numero et valeurs des GPIO devront etre remplace par les valeurs effectivement utilisees

    switch(direction){
        case 0:
            printf("la voiture va tout droit! \n");
            (*p_s_gpio).numero=0;
            (*p_s_gpio).valeur=0;
        break;

        case 1:
            printf("la voiture va à droite! \n");
            (*p_s_gpio).numero=0;
            (*p_s_gpio).valeur=1;
        break;


        case 2:
            printf("la voiture va à gauche! \n");
            (*p_s_gpio).numero=0;
            (*p_s_gpio).valeur=2;
        break;

        default:
            printf("erreur de direction! \n");
            (*p_s_gpio).numero=0;
            (*p_s_gpio).valeur=0;

    };


};

void avancerVoiture(int sens,struct RaspGpio* p_s_gpio){
//Les numero et valeurs des GPIO devront etre remplace par les valeurs effectivement utilisees

    switch(sens){
        case 0:
            //printf("la voiture n'avance pas! \n");
            (*p_s_gpio).numero=10;
            (*p_s_gpio).valeur=0;
        break;

        case 1:
            //printf("la voiture avance! \n");
            (*p_s_gpio).numero=10;
            (*p_s_gpio).valeur=1;
        break;


        case 2:
            //printf("la voiture recule! \n");
            (*p_s_gpio).numero=10;
            (*p_s_gpio).valeur=2;
        break;

        default:
            //printf("je ne sais pas quoi faire! \n");
            (*p_s_gpio).numero=10;
            (*p_s_gpio).valeur=0;

    };


};

//combinaison de deplacement
void* conduireVoiture(void* arg){

    DeplacementVoiture* deplacement = malloc(sizeof(DeplacementVoiture));
    deplacement = arg;
    struct RaspGpio* p_s_gpio = malloc(sizeof(RaspGpio));

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
        dirigerVoiture(direction,p_s_gpio);
        avancerVoiture(sens,p_s_gpio);

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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "inc/../thread_serveur_udp.h"
#include "inc/../raspberry_car_structures.h"
#include "inc/../thread_raspberry_car.h"



int main(int argc, char *argv[ ])
{
    if (argc != 2) { printf("Usage: port\n");
                    exit(1);}

    //port utilise par le serveur
    int port = atoi(argv[1]);
  //initialisation de la structure deplacement voiture
    DeplacementVoiture deplacementVoiture;
    deplacementVoiture.sens=0;
    deplacementVoiture.direction=0;

    //initialisation du mutex
    pthread_mutex_init(&deplacementVoiture.mutex,NULL);

    // structure de donnees a passer au serveur
    Thread_udp data_udp;
    data_udp.deplacementVoiture= &deplacementVoiture;
    data_udp.port= &port;

    //variables representant les threads
    pthread_t thread_serveur;
    pthread_t thread_deplacement;

    // creation des threads
    pthread_create(&thread_serveur, NULL, serveur_udp, &data_udp);
    pthread_create(&thread_deplacement, NULL, deplacement, &deplacementVoiture);

    //interruption du thread principal tant que les deux autres thread ne sont pas termines
    pthread_join(thread_serveur, NULL);
    pthread_join(thread_deplacement,NULL);

    return EXIT_SUCCESS;
}

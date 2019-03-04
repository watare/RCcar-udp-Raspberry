
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "../inc/thread_serveur_udp.h"
#include "../inc/thread_raspberry_car.h"

/* variables==================================================*/


/*entry points=======================================================*/
int main(int argc, char *argv[ ])
{
    if (argc != 2) { printf("Usage: port\n");exit(1);}

    //port utilise par le serveur
    int port = atoi(argv[1]);

    DeplacementVoiture s_deplacementvoiture;
    s_deplacementvoiture.sens=0;
    s_deplacementvoiture.direction=0;


    //initialisation du mutex
    pthread_mutex_init(&s_deplacementvoiture.mutex,NULL);

    // structure de donnees a passer au serveur
    DataUdp s_dataudp;
    s_dataudp.p_s_deplacementvoiture= &s_deplacementvoiture;
    s_dataudp.p_port= &port;

    //variables representant les threads
    pthread_t thread_serveur;
    pthread_t thread_deplacement;

    // creation des threads
    pthread_create(&thread_serveur, NULL, serveur_udp, &s_dataudp);
    pthread_create(&thread_deplacement, NULL, conduireVoiture, &s_deplacementvoiture);

    //interruption du thread principal tant que les deux autres thread ne sont pas termines
    pthread_join(thread_serveur, NULL);
    pthread_join(thread_deplacement,NULL);

    return EXIT_SUCCESS;
}

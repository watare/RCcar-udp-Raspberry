#ifndef STRUCTURES_VOITURE_H_INCLUDED
#define STRUCTURES_VOITURE_H_INCLUDED
//fichier contenant les differentes structures crées pour le projet

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

typedef struct Thread_udp{

    //port utilisee par le serveur udp
    int* port;

    //les donnees de deplacement
    DeplacementVoiture* deplacementVoiture;
}Thread_udp;
//la structure GPIO et l objet utilise pour ecrire sur les sorties physiques du raspberry
typedef struct Gpio {
    //numero de gpio
    int numero;
    //valeur a ecrire sur le GPIO
    int valeur;
}Gpio;



#endif // STRUCTURES_VOITURE_H_INCLUDED

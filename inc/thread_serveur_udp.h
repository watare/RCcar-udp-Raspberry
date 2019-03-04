#ifndef THREAD_SERVEUR_H_INCLUDED
#define THREAD_SERVEUR_H_INCLUDED

#include "thread_raspberry_car.h"
/*prototypes==============================================*/

void error(const char *msg);
void* serveur_udp(void * DataUdp);

/*structures==============================================*/

typedef struct DataUdp{

    //port utilisee par le serveur udp
    int* p_port;

    //les donnees de deplacement
    DeplacementVoiture* p_s_deplacementvoiture;
}DataUdp;

#endif // THREAD_SERVEUR_H_INCLUDED

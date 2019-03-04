/* Creates a datagram server.  The port
   number is passed as an argument.  This
   server runs forever */
//explication sur http://www.linuxhowtos.org/C_C++/socket.htm

#include "../inc/thread_raspberry_car.h"
#include "../inc/thread_serveur_udp.h"

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>

/*fonctions===========================================================*/
//fonction error
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
//fonction serveur udp
void* serveur_udp(void *arg)
{
    int sock, length, n;
   socklen_t s_fromlen;
   struct sockaddr_in s_server;
   struct sockaddr_in s_from;
   char sa_buf[10]={0};

   //recuperation du pointeur sur la structure et definition de son type
    DataUdp* p_s_dataudp = malloc(sizeof(DataUdp));
    p_s_dataudp=arg;

    //recuperation du port
    int* p_port=malloc(sizeof(int));
    p_port = (*p_s_dataudp).p_port;

    //recuperation des consignes de deplacement
    DeplacementVoiture* p_deplaceToi = (*p_s_dataudp).p_s_deplacementvoiture;



   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(s_server);
   bzero(&s_server,length);
   s_server.sin_family=AF_INET;
   s_server.sin_addr.s_addr=INADDR_ANY;
   s_server.sin_port=htons(*p_port);
   if (bind(sock,(struct sockaddr *)&s_server,length)<0)
       error("binding");
   s_fromlen = sizeof(struct sockaddr_in);
   while (1) {
       n = recvfrom(sock,sa_buf,1024,0,(struct sockaddr *)&s_from,&s_fromlen);

       if (n < 0) error("recvfrom");
       //write(1,"Received a datagram: ",21);

      //recuperation de la premiere valeur du tableau qui correspond a la direction
       char* direction=malloc(sizeof(char));
       direction = sa_buf[0];
       //printf("\n %i \n",atoi(&direction));


        //recuperation de la seconde valeur  qui correspond au sens de deplacement
       char* sens=malloc(sizeof(char));
       sens =sa_buf[1];

       //ecrire ici un verrouillage du mutex pour actualiser la variable deplacement voiture
       pthread_mutex_lock(&(*p_deplaceToi).mutex);
       //ecriture de la donnee reçue
       //write(1,"\n",1);
       //write(1,&sens,1);
        //mise a jour des consignes de deplacement
       (*p_deplaceToi).direction=atoi(&direction);
       //printf("\n %i \n",atoi(&direction));
       (*p_deplaceToi).sens=atoi(&sens);

        //liberation du mutex
       pthread_mutex_unlock(&(*p_deplaceToi).mutex);
       n = sendto(sock,"Got your message\n",17,
                  0,(struct sockaddr *)&s_from,s_fromlen);
       if (n  < 0) error("sendto");
   }

   //la tache est terminee on sort du thread
   pthread_exit(NULL);
 }

/* Creates a datagram server.  The port
   number is passed as an argument.  This
   server runs forever */
//explication sur http://www.linuxhowtos.org/C_C++/socket.htm

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>
#include "../inc/raspberry_car_structures.h"
//fonction error
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
//fonction serveur udp
void* serveur_udp(void *thread_udp)
{
   //recuperation du pointeur sur la structure et definition de son type
    Thread_udp* param = malloc(sizeof(Thread_udp));
    param=thread_udp;

    //recuperation du port
    int* port=malloc(sizeof(int));
    port = (*param).port;
    //recuperation des consignes de deplacement
    DeplacementVoiture* deplaceToi = (*param).deplacementVoiture;

   int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[10]={0};

   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(*port);
   if (bind(sock,(struct sockaddr *)&server,length)<0)
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   while (1) {
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);

       if (n < 0) error("recvfrom");
       //write(1,"Received a datagram: ",21);

      //recuperation de la premiere valeur du tableau qui correspond a la direction
       char* direction=malloc(sizeof(char));
       direction = buf[0];
       //printf("\n %i \n",atoi(&direction));


        //recuperation de la seconde valeur  qui correspond au sens de deplacement
       char* sens=malloc(sizeof(char));
       sens =buf[1];
        //printf("\n %i \n",atoi(&direction));

       //reinit du buffer
       //memset(buf, 0, sizeof buf);
    /*
       write(1,&direction,1);
       write(1,"\n",1);
       write(1,&sens,1);
       write(1,"\n",1);

       */
       //ecrire ici un verrouillage du mutex pour actualiser la variable deplacement voiture
       pthread_mutex_lock(&(*deplaceToi).mutex);
       //ecriture de la donnee reçue
       //write(1,"\n",1);
       //write(1,&sens,1);
        //mise a jour des consignes de deplacement
       (*deplaceToi).direction=atoi(&direction);
       //printf("\n %i \n",atoi(&direction));
       (*deplaceToi).sens=atoi(&sens);

        //liberation du mutex
       pthread_mutex_unlock(&(*deplaceToi).mutex);
       n = sendto(sock,"Got your message\n",17,
                  0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) error("sendto");
   }

   //la tache est terminee on sort du thread
   pthread_exit(NULL);
 }

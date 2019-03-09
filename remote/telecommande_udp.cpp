//GUI permettant de capter les fleches du clavier et d envoyer le resultat via une connexion UDP

#include <stdlib.h>
#include <stdio.h>

//pour la gestion des evenements
#include <SDL2/SDL.h>
#include <string>

//pour UDP
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>

void error(const char *);
void envoiMessage(int* sock, struct sockaddr_in* server,char direction, char sens);


/////////////////////////////////////////////////////////////////////

//La structure d'événements qu'on va utiliser
SDL_Event event;
//Récupération du keystates
const Uint8 *keystates = SDL_GetKeyboardState( NULL );
const char GAUCHE = '2';
const char DROITE = '1';
const char AVANT = '1';
const char ARRIERE = '2';
const char NEUTRE='0';

//variable envoye par le serveur
int sens = NEUTRE;
int direction = NEUTRE;

int main( int argc, char *argv[ ] )
{
        //Creation d un client udp
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;

    if (argc != 3) { printf("Usage: server port\n");
                    exit(1);
    }
    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("socket");

    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp==0) error("Unknown host");

    bcopy((char *)hp->h_addr,
        (char *)&server.sin_addr,
         hp->h_length);
    server.sin_port = htons(atoi(argv[2]));

    // variable pour quitter le programme
    bool quit = false;

    //cree la fenetre de gestion des evenements GUI

    SDL_Window *screen;
    if( SDL_Init( SDL_INIT_EVERYTHING) == -1 )
    {
        printf( "Can't init SDL:  %s\n", SDL_GetError( ) );
        return EXIT_FAILURE;
    }
    atexit( SDL_Quit );

    screen  = SDL_CreateWindow("ma fenetre",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             100, 100,
                             0);
    if( screen == NULL )
    {
        printf( "Can't set video mode: %s\n", SDL_GetError( ) );
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, 0);

    //SDL_CreateWindowAndRenderer(100, 100, 0, &screen, &renderer);

    SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

        //la partie gestion des evenements
    while(quit == false){

        sens = NEUTRE;
        direction = NEUTRE;
        while(SDL_PollEvent(&event) && quit == false){
            //sleep(1);
            //AVANT ARRIERE
            if ( keystates[SDL_SCANCODE_UP]) {
                sens = AVANT;
                SDL_PumpEvents();
                envoiMessage(&sock,&server,direction,sens);
            }else if(keystates[SDL_SCANCODE_DOWN]){
                sens = ARRIERE;
                SDL_PumpEvents();
                envoiMessage(&sock,&server,direction,sens);
            }else{
               sens = NEUTRE;
               SDL_PumpEvents();
            }

        //GAUCHE DROITE
            if ( keystates[SDL_SCANCODE_LEFT]) {
                direction = GAUCHE;
                SDL_PumpEvents();
                envoiMessage(&sock,&server,direction,sens);
            }else if(keystates[SDL_SCANCODE_RIGHT]){
                direction = DROITE;
                SDL_PumpEvents();
                envoiMessage(&sock,&server,direction,sens);
            }else{
               direction = NEUTRE;
               SDL_PumpEvents();
            }


            if (event.type == SDL_QUIT){
                quit = true;
            }

        };
    };

    //SDL_Delay( 3000 );

    return EXIT_SUCCESS;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void envoiMessage(int* sock, struct sockaddr_in* server,char direction, char sens){

    int n;
    unsigned int length=sizeof(struct sockaddr_in);
    char buffer[8];

    bzero(buffer,8);
    buffer[0] = direction;
    buffer[1] = sens;
    n=sendto(*sock,buffer,
    strlen(buffer),0,(const struct sockaddr *)server,length);

}

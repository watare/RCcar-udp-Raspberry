
CC=gcc
CFLAGS=-Wall -pthread
LDFLAGS=
EXEC=rc_raspberry


#variable des librairies

LIB1     = -lpigpio

LIB2     = -lpigpiod_if

LIB3     = -lpigpiod_if2

LIB = $(LIB1) $(LIB2) $(LIB3)

LL = 


all: $(EXEC) 

$(EXEC):  thread_raspberry_car.o thread_serveur_udp.o main_raspberry_car.o 
	$(CC) -o $(EXEC) main_raspberry_car.o thread_raspberry_car.o thread_serveur_udp.o ${LIB} $(CFLAGS)   

thread_raspberry_car.o : thread_raspberry_car.c
	$(CC) -o thread_raspberry_car.o -c thread_raspberry_car.c  $(LDFLAGS)

thread_serveur_udp.o : thread_serveur_udp.c
	$(CC) -o thread_serveur_udp.o -c thread_serveur_udp.c $(LDFLAGS)

main_raspberry_car.o : main_raspberry_car.c
	$(CC) -o main_raspberry_car.o -c main_raspberry_car.c $(LDFLAGS)
	
clean:
	rm  *.o

mrproper: clean
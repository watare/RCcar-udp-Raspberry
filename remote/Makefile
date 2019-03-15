
CC=g++
CFLAGS=-Wall -lSDL2
LDFLAGS=
EXEC=remote





all: $(EXEC) 

$(EXEC):  telecommande_udp.cpp 
	$(CC) telecommande_udp.cpp -o $(EXEC)   $(CFLAGS)   
	
clean:
	rm  *.o

mrproper: clean
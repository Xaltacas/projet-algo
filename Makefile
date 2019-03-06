EXEC=projet

all: $(EXEC)

projet: tas.o projet.o fonctions.o fonctions.h
	gcc -o projet -g tas.o projet.o fonctions.o -lm

tas.o: tas.c tas.h
	gcc -o tas.o -g -c tas.c -lm -Wall

fonctions.o: fonctions.c fonctions.h
	gcc -o fonctions.o -g -c fonctions.c -Wall

projet.o: projet.c tas.h 
	gcc -o projet.o -g -c projet.c -Wall

clean :
	rm *.o $(EXEC)	

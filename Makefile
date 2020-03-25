DIROBJ := obj/
DIREXE := exec/
DIRSRC := src/
DIRBOOKS := books/
DIRHEA := include/

INC := include/color.h include/searchResult.h

CFLAGS :=  -I$(DIRHEA) -c  -pthread -std=c++11
CC := g++

all : dirs searchResult SSOOIIGLE main

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

searchResult: 
	$(CC) -o $(DIROBJ)searchResult.o $(DIRSRC)searchResult.cpp $(CFLAGS) 

SSOOIIGLE: 
	$(CC) -o $(DIROBJ)SSOOIIGLE.o $(DIRSRC)SSOOIIGLE.cpp $(CFLAGS) 

main:
	$(CC) -o $(DIREXE)SSOOIIGLE $(DIROBJ)SSOOIIGLE.o $(DIROBJ)searchResult.o -pthread -std=c++11

test:
	./$(DIREXE)SSOOIIGLE books/21_leyes_del_liderazgo.txt sucedió 5

test1:
	./$(DIREXE)SSOOIIGLE books/17_leyes_del_trabajo_en_equipo.txt leyes 3
clean:
	rm -r $(DIREXE) $(DIROBJ)
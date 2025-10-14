solitaire: main.o solitaire.o
	g++ -g main.o solitaire.o -o solitaire

main.o: main.cpp solitaire.h 
	g++  -Wall -g -c main.cpp -o main.o

solitaire.o: solitaire.cpp solitaire.h
	g++ -Wall -g -c solitaire.cpp  -o solitaire.o

clean:
	rm -f *.o solitaire
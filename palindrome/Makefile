word: main.o words.o
	g++ -g main.o words.o -o words

main.o: main.cpp words.h 
	g++  -Wall -g -c main.cpp -o main.o

words.o: words.cpp words.h
	g++ -Wall -g -c words.cpp  -o words.o

clean:
	rm -f *.o words
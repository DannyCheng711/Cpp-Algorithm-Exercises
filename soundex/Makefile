soundex: main.o soundex.o
	g++ -g main.o soundex.o -o soundex

main.o: main.cpp soundex.h 
	g++  -Wall -g -c main.cpp -o main.o

soundex.o: soundex.cpp soundex.h
	g++ -Wall -g -c soundex.cpp  -o soundex.o

clean:
	rm -f *.o soundex
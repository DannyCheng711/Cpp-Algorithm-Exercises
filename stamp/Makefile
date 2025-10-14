stamp: main.o stamp.o
	g++ -g main.o stamp.o -o stamp -lcrypto

main.o: main.cpp stamp.h 
	g++  -Wall -g -c main.cpp -o main.o -lcrypto

stamp.o: stamp.cpp stamp.h
	g++ -Wall -g -c stamp.cpp  -o stamp.o -lcrypto

clean:
	rm -f *.o scrabble
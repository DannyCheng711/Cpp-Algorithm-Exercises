laser: main.o laser.o
	g++ -g main.o laser.o -o laser

main.o: main.cpp laser.h 
	g++  -Wall -g -c main.cpp -o main.o

laser.o: laser.cpp laser.h
	g++ -Wall -g -c laser.cpp  -o laser.o

clean:
	rm -f *.o laser
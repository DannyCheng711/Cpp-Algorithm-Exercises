sokoban: main.o sokoban.o
	g++ -g -O3 main.o sokoban.o -o sokoban

main.o: main.cpp sokoban.h 
	g++ -Wall -O3 -g -c main.cpp -o main.o

sokoban.o: sokoban.cpp sokoban.h
	g++ -Wall -O3 -g -c sokoban.cpp  -o sokoban.o

clean:
	rm -f *.o sokoban
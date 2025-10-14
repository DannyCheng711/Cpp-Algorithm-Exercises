execute: main.o gogen.o
	g++ -g main.o gogen.o -o execute

main.o: main.cpp common.h mask.h gogen.h
	g++  -Wall -g -c main.cpp -o main.o

gogen.o: gogen_review.cpp common.h mask.h gogen.h
	g++ -Wall -g -c gogen_review.cpp  -o gogen.o

clean:
	rm -f *.o execute

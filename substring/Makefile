substring: main.o substring.o
	g++ -g main.o substring.o -o substring

main.o: main.cpp substring.h 
	g++  -Wall -g -c main.cpp -o main.o

substring.o: substring_review.cpp substring.h
	g++ -Wall -g -c substring_review.cpp  -o substring.o

clean:
	rm -f *.o substring
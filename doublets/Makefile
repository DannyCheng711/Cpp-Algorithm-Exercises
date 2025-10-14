doublets: main.o doublets.o
	g++ -g main.o doublets.o -o doublets

main.o: main.cpp doublets.h dictionary.h
	g++ -Wall -g -c main.cpp -o main.o

doublets.o: doublets.cpp doublets.h dictionary.h
	g++ -Wall -g -c doublets.cpp  -o doublets.o

clean:
	rm -f *.o doublets
FILES = header.h print.h

proyecto1:	proyecto1.cpp
		g++ -Wall -std=c++11 -o main $(FILES) proyecto1.cpp

proyecto2:	proyecto2.cpp
		g++ -Wall -std=c++11 -o main $(FILES) dfsbnb.h proyecto2.cpp

clean:
		rm -f main core *~  */*/*.txt
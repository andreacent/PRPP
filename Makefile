FILES = header.h print.h main.cpp

main:	main.cpp
		g++ -Wall -std=c++11 -o main $(FILES)

clean:
		rm -f main core *~  */*/*.txt
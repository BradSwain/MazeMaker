all: Maze

Maze: main.cpp maze.o
	g++ -std=c++11 main.cpp -o Maze -lncurses maze.o

maze.o: maze.cpp maze.h
	g++ -std=c++11 -c maze.cpp
	
clean:
	rm *.o Maze


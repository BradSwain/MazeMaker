all: Maze

Maze: main.cpp maze.o
	g++ -std=c++11 main.cpp -o Maze -lncurses -pthread maze.o SafeQueue.o

maze.o: maze.cpp maze.h SafeQueue.o
	g++ -std=c++11 -c maze.cpp

SafeQueue.o: SafeQueue.cpp SafeQueue.h
	g++ -std=c++11 -c SafeQueue.cpp
	
clean:
	rm *.o Maze


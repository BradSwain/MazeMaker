#ifndef MAZE_HEADER
#define MAZE_HEADER

#define MAZE_DEFAULT_SIZE 10
#define MAZE_DEFAULT_STEP 10

#include <stack>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <iostream>

#include <chrono>
#include <thread>
#include <ncurses.h>

#include "SafeQueue.h"

class Maze {
protected:
    struct Node {
        bool down, left, visited;
        Node() : down(false), left(false), visited(false){}
    };
    
    Node* maze;
    int rows, cols;
    
    bool is_idx_top(int i);
    bool is_idx_bottom(int i);
    bool is_idx_right(int i);
    bool is_idx_left(int i);
    
    int check_up(int idx, std::stack<int>& stack);
    int check_down(int idx, std::stack<int>& stack);
    int check_left(int idx, std::stack<int>& stack);
    int check_right(int idx, std::stack<int>& stack);
    int get_next(int idx, std::stack<int>& stack);
    
    void build_maze();
    
public:
    Maze(int _cols = MAZE_DEFAULT_SIZE, int _rows = MAZE_DEFAULT_SIZE) : cols(_cols), rows(_rows) {
        srand(time(NULL));
        int len = rows * cols;
        maze = new Node[len];
    }
    ~Maze() {
        delete []maze;
    }
    
    std::string to_string();
};

class MazeCurses : public Maze {
    
    SafeQueue draw_buffer;
    int step_time;
    
    void build_maze();
    void queue_draw(int i, int color);
    void draw();
    void draw_empty_board();
public:
    MazeCurses(int _cols = MAZE_DEFAULT_SIZE, int _rows = MAZE_DEFAULT_SIZE, int _s = MAZE_DEFAULT_STEP) 
    : Maze(_cols, _rows), step_time(_s) {
        
        if (!stdscr) {
            std::cout<< "Call init screen to make use of MazeCurses" << std::endl;
            return;
        }

        std::thread builder(&MazeCurses::build_maze, this);
        draw_empty_board();
        std::thread reader(&MazeCurses::draw, this);
        
        builder.join();
        reader.join();
    }
};

#endif
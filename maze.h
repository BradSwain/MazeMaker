#ifndef MAZE_HEADER
#define MAZE_HEADER

#define MAZE_DEFAULT_SIZE 10

#include <stack>
#include <string>
#include <cstdlib> 
#include <ctime> 

class Maze {
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
    
    void draw_idx(int i, bool recurse);
    
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
        
        build_maze();
    }
    ~Maze() {
        delete []maze;
    }
    
    std::string to_string();
};

#endif
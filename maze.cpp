#include <stack>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <string>

class Maze {
    
    struct Node {
        bool up, down, left, right, visited;
        int index;
        Node() : up(false), down(false), left(false), right(false), visited(false){}
    };
    
    Node* maze;
    int size;
    std::stack<int> stack;
    std::string string_representation;
    
    
    int check_up(int idx) {
        if (idx > size && !maze[idx - size].visited) {
            maze[idx].up = true;
            stack.push(idx);

            idx -= size;
            maze[idx].visited = true;
            maze[idx].down = true;
            
            return idx;
        }
        
        return -1;
    }
    
    int check_down(int idx) {
        if (idx < size * (size-1) && !maze[idx + size].visited) {
            maze[idx].down = true;
            stack.push(idx);

            idx += size;
            maze[idx].visited = true;
            maze[idx].up = true;
            
            return idx;
        }
        
        return -1;
    }
    
    int check_left(int idx) {
        if (idx % size != 0 && !maze[idx - 1].visited) {
            maze[idx].left = true;
            stack.push(idx);

            idx -= 1;
            maze[idx].visited = true;
            maze[idx].right = true;
            
            return idx;
        }
        
        return -1;
    }
    
    int check_right(int idx) {
        if ((idx+1) % size != 0 && !maze[idx + 1].visited) {
            maze[idx].right = true;
            stack.push(idx);

            idx += 1;
            maze[idx].visited = true;
            maze[idx].left = true;
            
            return idx;
        }
        
        return -1;
    }
    
    int check_next(int idx) {
        int orig = rand()%4;
        int curr = orig;
        int next_idx = -1;
        
        do {
            switch(curr) {
                case 0:
                    next_idx = check_up(idx);
                    curr++;
                break;
                case 1:
                    next_idx = check_down(idx);
                    curr++;
                break;
                case 2:
                    next_idx = check_left(idx);
                    curr++;
                break;
                case 3:
                    next_idx = check_right(idx);
                    curr = 0;
                break;
            }
        } while (next_idx == -1 && orig != curr);
        
        if (next_idx == -1 && !stack.empty()) {
            int n = stack.top();
            stack.pop();
            next_idx = check_next(n);
        }
        
        return next_idx;
    }
    
    std::string create_string() {
        std::string str = "";
        for (int col = 0; col < size*2 +1; col++) {
           str += "_";
        }
        str += "\n";
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                int i = size*row + col;
                str += maze[i].left? ' ' : '|';
                str += maze[i].down? ' ' : '_';
            }
            str += "|\n";
        }
        
        return str;
    }
  
public:

    std::string to_string(){return string_representation;}
    
    Maze(int _size = 10) : size(_size) {
        int len = size * size;
        maze = new Node[len];
        for (int i = 0; i < len; i++) {
            maze[i].index = i;
        }
        
        maze[0].visited = true;
        stack.push(0);
        int current_idx = 0;
        while(current_idx != -1) {
            current_idx = check_next(current_idx);
        }
        
        string_representation = create_string(); 
    }
};

int main() {
    srand(time(NULL));
    Maze m(10);
    std::cout << m.to_string();
}
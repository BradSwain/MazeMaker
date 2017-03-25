#include "maze.h"

bool Maze::is_idx_top(int i) {
        return i < cols;
    }
bool Maze::is_idx_bottom(int i) {
    static const int first_bottom_index = (rows-1) * cols;
    return i >= first_bottom_index;
}
bool Maze::is_idx_right(int i) {
    return (i+1) % cols == 0;
}
bool Maze::is_idx_left(int i) {
    return i % cols == 0;
}

void Maze::draw_idx(int i, bool recurse = true) {
    /*char middle[4];
    middle[3] = '\0';
    if (is_idx_bottom(i)) {
        middle[0] = maze[i].left? '_' : '|';
        middle[1] = '_';
        middle[2] = maze[i].right? '_' : '|';
    } else {
        middle[0] = maze[i].left? ' ' : '|';
        middle[1] = maze[i].down? ' ' : '_';
        middle[2] = maze[i].right? ' ' : '|';
    }
    mvprintw(maze[i].row + 1, maze[i].col*2, middle);
    
    if (recurse) {
        if (!is_idx_bottom(i)) {
            draw_idx(i + size, false);
        }
        if (!is_idx_top(i)) {
            draw_idx(i - size, false);
        }
    }
    
    refresh();*/
}

int Maze::check_up(int idx, std::stack<int>& stack) {
    if (!is_idx_top(idx) && !maze[idx - cols].visited) {
        stack.push(idx);

        idx -= cols;
        maze[idx].visited = true;
        maze[idx].down = true;
        
        return idx;
    }
    
    return -1;
}
int Maze::check_down(int idx, std::stack<int>& stack) {
    if (!is_idx_bottom(idx) && !maze[idx + cols].visited) {
        maze[idx].down = true;
        stack.push(idx);

        idx += cols;
        maze[idx].visited = true;

        return idx;
    }
    
    return -1;
}
int Maze::check_left(int idx, std::stack<int>& stack) {
    if (!is_idx_left(idx) && !maze[idx - 1].visited) {
        maze[idx].left = true;
        stack.push(idx);

        idx -= 1;
        maze[idx].visited = true;

        return idx;
    }
    
    return -1;
}
int Maze::check_right(int idx, std::stack<int>& stack) {
    if (!is_idx_right(idx) && !maze[idx + 1].visited) {
        stack.push(idx);

        idx += 1;
        maze[idx].visited = true;
        maze[idx].left = true;
        
        return idx;
    }
    
    return -1;
}
int Maze::get_next(int idx, std::stack<int>& stack) {
    int orig = rand()%4;
    int curr = orig;
    int next_idx = -1;
    
    do {
        switch(curr) {
            case 0:
                next_idx = check_up(idx, stack);
                curr++;
            break;
            case 1:
                next_idx = check_down(idx, stack);
                curr++;
            break;
            case 2:
                next_idx = check_left(idx, stack);
                curr++;
            break;
            case 3:
                next_idx = check_right(idx, stack);
                curr = 0;
            break;
        }
    } while (next_idx == -1 && orig != curr);
    
    return next_idx;
}

void Maze::build_maze() {
    std::stack<int> stack;
    
    maze[0].visited = true;
    stack.push(0);
    int current_idx = 0;
    while(current_idx != -1) {
        current_idx = get_next(current_idx, stack);
        while (current_idx == -1 && !stack.empty()) {
            int n = stack.top();
            stack.pop();
            current_idx = get_next(n, stack);
        }
    }
}

std::string Maze::to_string() {
        static std::string str = "";
        if (!str.empty()) {
            return str;
        }
        
        //else actually create the string
        for (int col = 0; col < cols*2 +1; col++) {
           str += "_";
        }
        str += "\n";
        for (int row = 0; row < rows-1; row++) {
            for (int col = 0; col < cols; col++) {
                int i = cols*row + col;
                str += maze[i].left? ' ' : '|';
                str += maze[i].down? ' ' : '_';
            }
            str += "|\n";
        }
        for (int col = 0; col < cols; col++) {
            int i = cols*(rows-1) + col;
            str += maze[i].left? '_' : '|';
            str += maze[i].down? ' ' : '_';
        }
        str += "|\n";
        
        return str;
    }
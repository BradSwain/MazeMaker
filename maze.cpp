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
        
        //build the maze if not yet built
        if (!maze[0].visited) {
            build_maze();
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
    


void MazeCurses::queue_draw(int i, int color = 1) {
    PrintData* data = new PrintData();
    data->color = color;
    data->row = i/cols;
    data->col = (i%cols)*2 + 1 ;

    char empty_delimiter = is_idx_bottom(i)? '_' : ' ';
    //left, bottom, right
    data->str[0] = maze[i].left? empty_delimiter : '|';
    data->str[1] = maze[i].down? empty_delimiter : '_';
    data->str[2] = !is_idx_right(i) && maze[i+1].left? empty_delimiter : '|';
    //top
    data->str[3] = !is_idx_top(i) && maze[i-cols].down? ' ' : '_';
    
   draw_buffer.write(data);
}

void MazeCurses::draw() {
    const PrintData* data = draw_buffer.read();
    while (data != PrintData::kill) {
        char buff[4];
        buff[0] = data->str[3];
        buff[1] = '\0';
        mvprintw(data->row, data->col, buff);
        
        buff[0] = data->str[0];
        buff[1] = data->str[1];
        buff[2] = data->str[2];
        buff[3] = '\0';
        mvprintw(data->row+1, data->col-1, buff);
        
        refresh();
        
        delete data;
        data = draw_buffer.read();
        std::this_thread::sleep_for(std::chrono::milliseconds(step_time));
    }
    
    move(1,1);
}

void MazeCurses::draw_empty_board() {
    move(0,0);
    std::string top(cols*2 + 1, '_');
    top += '\n';
    std::string side = "";
    for (int i = 0; i < cols; i++) {
        side += "|_";
    }
    side += "|\n";
    
    printw(top.c_str());
    for (int i = 0; i < cols; i++) {
        printw(side.c_str());
    }
    refresh();
}
    
void MazeCurses::build_maze() {
    std::stack<int> stack;
    
    maze[0].visited = true;
    stack.push(0);
    int current_idx = 0;
    while(current_idx != -1) {
        queue_draw(current_idx);

        current_idx = get_next(current_idx, stack);
        while (current_idx == -1 && !stack.empty()) {
            int n = stack.top();
            stack.pop();
            current_idx = get_next(n, stack);
        }
    }
    
    draw_buffer.write(PrintData::kill);
}
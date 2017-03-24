#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <chrono>
#include <thread>
#include <time.h>

#define DEFAULT_SIZE 10
#define DEFAULT_STEP 100
int GLOB_SIZE;
int GLOB_SLOW_STEP;

void read_input_flags(int argc, char* argv[]) {
    GLOB_SIZE = DEFAULT_SIZE;
    GLOB_SLOW_STEP = DEFAULT_STEP;
    if (argc > 1) {
        int i = 0;
        for (i = 1; i < argc; i++) {
            if (argv[i][0] == '-' && argv[i][1] == 's') {
                GLOB_SLOW_STEP = DEFAULT_STEP;
                std::string flag = std::string(argv[i]);
                if (flag.size() > 2 && flag[2] == '=') {
                    try {
                        GLOB_SLOW_STEP = std::stoi(flag.substr(3));
                    } catch (...) {/*Only dreams now*/}
                }
                break;
            }
        }
        
        try {
            GLOB_SIZE = std::stoi(argv[argc-1]);
            struct winsize win_size;
            ioctl(STDOUT_FILENO,TIOCGWINSZ,&win_size);
            int max_size = (win_size.ws_col-1)/2;
            GLOB_SIZE = GLOB_SIZE > max_size? max_size : GLOB_SIZE;
        } catch(...) {/*ssshhh*/}
    }
}
void draw_empty_board() {
    std::string top(GLOB_SIZE*2 + 1, '_');
    top += '\n';
    std::string side = "";
    for (int i = 0; i < GLOB_SIZE; i++) {
        side += "|_";
    }
    side += "|\n";
    
    printw(top.c_str());
    for (int i = 0; i < GLOB_SIZE; i++) {
        printw(side.c_str());
    }
    refresh();
}


class Maze {
    struct Node {
        bool up, right, down, left, visited;
        int index, row, col;
        
        Node() : up(false), right(false), down(false), left(false), visited(false){}
        void set_index(int i) {
            index = i;
            row = index / GLOB_SIZE;
            col = index % GLOB_SIZE;
            if (index != GLOB_SIZE * row + col) {
                std::cout << "ERROR\n";
            }
        }
    };
    
    Node* maze;
    int size;
    std::stack<int> stack;
    
    bool is_idx_top(int i) {
        return i < size;
    }
    bool is_idx_bottom(int i) {
        static const int max_non_bottom_index = size * (size-1) - 1;
        return i > max_non_bottom_index;
    }
    bool is_idx_right(int i) {
        return (i+1) % size == 0;
    }
    bool is_idx_left(int i) {
        return i % size == 0;
    }
    
    void draw_idx(int i, bool recurse = true) {
        char middle[4];
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
        
        refresh();
    }
    
    int check_up(int idx) {
        if (!is_idx_top(idx) && !maze[idx - size].visited) {
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
        if (!is_idx_bottom(idx) && !maze[idx + size].visited) {
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
        if (!is_idx_left(idx) && !maze[idx - 1].visited) {
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
        if (!is_idx_right(idx) && !maze[idx + 1].visited) {
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
        
        draw_idx(idx);
        std::this_thread::sleep_for(std::chrono::milliseconds(GLOB_SLOW_STEP));
        
        if (next_idx == -1 && !stack.empty()) {
            int n = stack.top();
            stack.pop();
            next_idx = check_next(n);
        }
        
        return next_idx;
    }
    
    
    
public:
    Maze(int _size = DEFAULT_SIZE) : size(_size) {
        int len = size * size;
        maze = new Node[len];
        for (int i = 0; i < len; i++) {
            maze[i].set_index(i);
        }
        
        maze[0].visited = true;
        stack.push(0);
        int current_idx = 0;
        while(current_idx != -1) {
            current_idx = check_next(current_idx);
        }
    }
    ~Maze() {
        delete []maze;
    }
    
    std::string create_string() {
        std::string str = "";
        for (int col = 0; col < size*2 +1; col++) {
           str += "_";
        }
        str += "\n";
        for (int row = 0; row < size-1; row++) {
            for (int col = 0; col < size; col++) {
                int i = size*row + col;
                str += maze[i].left? ' ' : '|';
                str += maze[i].down? ' ' : '_';
            }
            str += "|\n";
        }
        for (int col = 0; col < size; col++) {
            int i = size*(size-1) + col;
            str += maze[i].left? '_' : '|';
            str += maze[i].down? ' ' : '_';
        }
        str += "|\n";
        
        return str;
    }
};

int main(int argc, char* argv[]) {
    read_input_flags(argc, argv);
    srand(time(NULL));
    
    
    initscr();
    curs_set(0);
    draw_empty_board();
    Maze m(GLOB_SIZE);
    move(1,1);
    refresh();
    getch();
    endwin();
    
    std::cout<<m.create_string();
    
    return 0;
}
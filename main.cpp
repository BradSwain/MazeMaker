#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include "maze.h"

const int DEFAULT_STEP = 100;
const int STANDARD_SIZE = 10;
int GLOB_COLS = STANDARD_SIZE;
int GLOB_ROWS = STANDARD_SIZE;
int GLOB_SLOW_STEP = DEFAULT_STEP;
int GLOB_ENDLESS = 0;

void read_input_flags(int argc, char* argv[]) {
    if (argc > 1) {
        int i = 0;
        for (i = 1; i < argc; i++) {
            std::string arg = std::string(argv[i]);
            if (arg[0] == '-' && arg.size() > 1) {
                switch(arg[1]) {
                     //Endless
                    case 'e':
                        GLOB_ENDLESS = 1;
                    break;
                    //Step
                    case 's':
                        GLOB_SLOW_STEP = DEFAULT_STEP;
                        if (arg.size() > 2 && arg[2] == '=') {
                            try {
                                GLOB_SLOW_STEP = std::stoi(arg.substr(3));
                            } catch (...) {/*Sssshhhh*/}
                        }
                    break;
                    //set both rows and cols
                    case 'x':
                        if (arg.size() > 2 && arg[2] == '=') {
                            try {
                                GLOB_COLS = std::stoi(arg.substr(3));
                                GLOB_ROWS = GLOB_COLS;
                            } catch (...) {/*Sssshhhh*/}
                        }
                    break;
                    //set rows
                    case 'r':
                        if (arg.size() > 2 && arg[2] == '=') {
                            try {
                                GLOB_ROWS = std::stoi(arg.substr(3));
                            } catch (...) {/*Sssshhhh*/}
                        }
                    break;
                    //set cols
                    case 'c':
                        if (arg.size() > 2 && arg[2] == '=') {
                            try {
                                GLOB_COLS = std::stoi(arg.substr(3));
                            } catch (...) {/*Sssshhhh*/}
                        }
                    break;
                    default:
                        std::cout << arg << " is not a recognised flag.\t\t\tYou fool!\n";
                    break;
                }
            }
        }
        //Try and parse last item as int to be used as row/col value
        if (argv[argc-1][0] != '-') {
            try {
                GLOB_COLS = std::stoi(std::string(argv[argc-1]));
                GLOB_ROWS = GLOB_COLS;
            } catch (...) {/*Sssshhhh*/}
        }
        
        //ensure maze is not bigger than screen
        if (GLOB_COLS != STANDARD_SIZE | GLOB_ROWS != STANDARD_SIZE) {
            struct winsize win_size;
            ioctl(STDOUT_FILENO,TIOCGWINSZ,&win_size);
            int max_cols = (win_size.ws_col-1)/2;
            int max_rows = win_size.ws_row-2;

            GLOB_COLS = GLOB_COLS > max_cols? max_cols : GLOB_COLS;
            GLOB_ROWS = GLOB_ROWS > max_rows? max_rows : GLOB_ROWS;
        }
    }
}

int main(int argc, char* argv[]) {
    read_input_flags(argc, argv);
    
    initscr();
    curs_set(0);
    Maze* m = nullptr;
    do {
        delete m; //stack overflow this should be fine on nullptr...we shall see...
        m = new MazeCurses(GLOB_COLS, GLOB_ROWS, GLOB_SLOW_STEP);
    } while(GLOB_ENDLESS);
    
    getch();
    endwin();
    
    std::cout<<m->to_string();
    
    return 0;
}
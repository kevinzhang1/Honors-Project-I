#include <iostream>
#include <fstream>
#include <string>
#include "translator.h"

#ifndef COMMAND
#define COMMAND

class command {
private:
    string path;
    void splice(const char* file_name);

public:
    command();
    void main_menu(int* q);
    void set_path();
    void compile_splice();
    void racket_run();
    void translate();
    void help();
    void view_path_folder();
    void info();
};

#endif

#include <any>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>

#ifndef TRANSLATOR
#define TRANSLATOR

using namespace std;


class low_translator {
private:
    vector<string> output;
    string Path;
    enum var_state {
        None, get_var_name, get_var_variable
    };
    var_state v_state;
    struct swift_var {
        string name;
        enum datatypes {integer, floats, boolean, String};
        datatypes type;
        string value;
    };
    swift_var s_var;
public:
    low_translator(string path, ifstream* file);
    void main_interpretor(string line);
    void swift_variable_interpretor(var_state state, string line); 
    string racket_variable_translator(swift_var var);
    void write_racket_file();

};

#endif

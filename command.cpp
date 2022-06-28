#include "command.h"

using namespace std;

/*
command::command() This constructor repeats the main_menu function until the user quits
Inputs: None
*/

command::command() {
    ifstream file("FilePath.txt");
    file >> path;
    file.close();
    int q = 0;
    while (q == 0) main_menu(&q);
}

/*
void command::main_menu(int* q) This function displays the main menu, reads, and interprets the user's command and activates its respective function
Input: int* q
*/

void command::main_menu(int* q) {
    string command;
    cout << "What do you wish to do? Press H for help, Q to quit: ";
    cin >> command;
    if (command == "H") {
        help();
    }
    else if (command == "I") {
        cout << "You have selected to get info." << endl;
        info();
    }
    else if (command == "Q") {
        cout << "You have selected to quit." << endl;
        *q = 1;
    }
    else if (command == "SP") {
        cout << "You have selected to set path." << endl;
        set_path();
    }
    else if (command == "CS") {
        cout << "You have selected to compile and splice." << endl;
        compile_splice();
    }
    else if (command == "RR") {
        cout << "You have selected to run a racket file." << endl;
        racket_run();
    }
    else if (command == "T") {
        cout << "You have selected to translate." << endl;
        translate();
    }
    else if (command == "VPF") {
        cout << "You have selected to view path folder." << endl;
        view_path_folder();
    }
    else { cout << "You have entered an invalid command. Please try again." << endl; }
}

/*
void command::splice(const char* file_name) Splices the Swift Intermediate Language file and only keeps the part useful for translation. The real Sil file is generally thousands of lines long but this program reduces the number of lines to only a few dozen
Input: const char* file_name
*/

void command::splice(const char* file_name) {
    ifstream fin(file_name);
    ofstream fout;
    string line;
    fout.open("temp.txt", ios::out);
    while (getline(fin, line)) if (line == "// main") { fout << line << endl; break; }
    while (getline(fin, line)) {
        if (line == "} // end sil function 'main'") { fout << line << endl; break; }
        else fout << line << endl;
    }
    fout.close(); fin.close();
    remove(file_name);
    rename("temp.txt", file_name);
}

/*
void command::set_path() This command allows the user to change and modify the file path the program is working with
Inputs: None
*/

void command::set_path() {
    string filepath;
    cout << "Current file path is: " << path << endl;
    cout << "Which path folder are you looking to save?: ";
    cin >> filepath;
    if (filepath == "back") return;
    ofstream ofile("FilePath.txt");
    ofile << filepath;
    cout << "Saved path folder as: " << filepath;
    ofile.close();
}

/*
void command::compile_splice() This command compiles a Swift file into Sil and then splices it
Inputs: None
*/

void command::compile_splice() {
    string fileName;
    cout << "\nWhich Swift file do you want to compile into sil?: ";
    cin >> fileName;
    if (fileName == "back") return;
    ifstream file(path + fileName + ".swift");
    while (!file.is_open()) {
        cout << "That file doesn't exist, try again: ";
        cin >> fileName;
        if (fileName == "back") return;
        file.open(fileName);
    }
    string command = "swiftc -emit-sil " + path + fileName + ".swift >" + path + fileName + ".txt";
    system(command.c_str());
    cout << "Output is saved into " << fileName + ".txt\n";
    splice((path + fileName + ".txt").c_str());
    system(("notepad " + path + fileName + ".txt").c_str());
    file.close();
}

/*
void command::racket_run() This command prompts the user to enter a racket program name and the function will run it
Inputs: None
*/

void command::racket_run() {
    string fileName, ifsave, command;
    cout << "Which racket file do you want to run?: ";
    cin >> fileName;
    if (fileName == "back") return;
    ifstream file(path + fileName + ".rkt");
    while (!file.is_open()) {
        cout << "That file doesn't exist, try again: ";
        cin >> fileName;
        if (fileName == "back") return;
        file.open(path + fileName + ".rkt");
    }
    file.close();
    cout << "Do you want to save output into text file?: ";
    cin >> ifsave;
    if (ifsave == "back") return;
    while (ifsave != "yes" && ifsave != "no") {
        cout << "That input is not valid, try again: ";
        cin >> ifsave;
        if (ifsave == "back") return;
    }
    if (ifsave == "yes") {
        system(("racket " + path + fileName + ".rkt > " + path + fileName + "_output.txt").c_str());
        cout << "Output is saved into " << fileName + ".txt\n";
    }
    else {
        system(("racket " + path + fileName + ".rkt").c_str());
    }
}

/*
void command::translate() This command translates a Sil text file into Racket
Inputs: None
*/

void command::translate() {
    string input;
    cout << "Which file do you desire to translate? (.txt files only): ";
    cin >> input;
    if (input == "back") return;
    ifstream file(path + input + ".txt");
        while (!file.is_open()) {
            cout << "That file doesn't exist, try again: ";
            cin >> input;
            if (input == "back") return;
            file.open(path + input + ".txt");
        }
        cout << "Will run translator, output will be saved to racket_result.rkt in path: " << path << endl;
        low_translator l(path, &file);
}

/*
void command::help() This command prints out the help menu for the user
Inputs: None
*/

void command::help() {
    cout << "Here are the available commands and their functions." << endl;
    cout << "H-Help" << endl;
    cout << "I-Info" << endl;
    cout << "Q-Quit" << endl;
    cout << "CS-Compile and Splice" << endl;
    cout << "SP-Set Path" << endl;
    cout << "T-Translate" << endl;
}

/*
void command::view_path_folder() This command allows the user to view the current file path
Inputs: None
*/

void command::view_path_folder() {
    system(("dir " + path).c_str());
}

/*
void command::info() This command gives some information on the translator. It prints out the below statements when called upon
Inputs: None
*/

void command::info() {
    cout << "This is a program to translate Swift to Racket. It features a low level translator." << endl;
    cout << "For the low level translator: " << endl;
    cout << "Inputs: Only accepts sil files in .txt file format" << endl;
    cout << "Syntax information: Only translates variable declarations" << endl;
}
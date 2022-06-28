// main() is where program execution begins.
#include "command.h"
#include "translator.h"

using namespace std;

int main() {
    command c;
    string path;
    ifstream files("FilePath.txt");
    files >> path;
    files.close();
    ifstream file(path + "Example1.txt");
    low_translator l(path, &file);
    return 0;
}
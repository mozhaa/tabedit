#include "editor.h"
#include <fstream>
#include <string>

using namespace std;

void show_usage() {
    printf("Usage: ./tabedit {filename}\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        show_usage();
        return 1;
    }
    string filename = argv[1];
    tabedit::run(filename);
    return 0;
}
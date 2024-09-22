#include "editor.h"
#include <string>
#include <locale.h>

using namespace std;

void show_usage() {
    printf("Usage: ./tabedit {filename}\n");
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");
    string filename;
    if (argc == 1) {
        filename = "";
    } else {
        filename = argv[1];
    }
    tabedit::run(filename);
    return 0;
}
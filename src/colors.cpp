#include "colors.h"

namespace tabedit {

void init_colors() {
    start_color();

    init_color(C_BW_0000, 000, 000, 000);
    init_color(C_BW_0100, 100, 100, 100);
    init_color(C_BW_0200, 200, 200, 200);
    init_color(C_BW_0300, 300, 300, 300);
    init_color(C_BW_0400, 400, 400, 400);
    init_color(C_BW_0500, 500, 500, 500);
    init_color(C_BW_0600, 600, 600, 600);
    init_color(C_BW_0700, 700, 700, 700);
    init_color(C_BW_0800, 800, 800, 800);
    init_color(C_BW_0900, 900, 900, 900);
    init_color(C_BW_1000, 1000, 1000, 1000);

    init_pair(CP_MAIN_1, C_FG, C_BG_1);
    init_pair(CP_MAIN_2, C_FG, C_BG_2);
    init_pair(CP_MAIN_3, C_FG, C_BG_3);
    init_pair(CP_NOTE_1, C_BG, C_BW_0800);
    init_pair(CP_NOTE_2, C_BG, C_BW_0900);
    init_pair(CP_DEFAULT, C_FG, C_BG);
    init_pair(CP_CURSOR, C_BG, C_FG);
    init_pair(CP_SELECTION, C_BG, C_BW_0900);

    default_window(stdscr);
}

void default_window(WINDOW* win) {
    wbkgd(win, COLOR_PAIR(CP_DEFAULT));
}

}
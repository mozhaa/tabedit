#pragma once

#include <ncursesw/ncurses.h>

namespace tabedit {

// Raw colors
#define C_BW_0000 1
#define C_BW_0100 2
#define C_BW_0200 3
#define C_BW_0300 4
#define C_BW_0400 5
#define C_BW_0500 6
#define C_BW_0600 7
#define C_BW_0700 8
#define C_BW_0800 9
#define C_BW_0900 10
#define C_BW_1000 11
#define C_YELLOW 12
#define C_PINK 13
#define C_LIGHT_YELLOW 14

// Named colors
#define C_BG C_BW_0000
#define C_FG C_BW_1000
#define C_BG_1 C_BW_0300
#define C_BG_2 C_BW_0200
#define C_BG_3 C_BW_0100

// Color pairs
#define CP_MAIN_1 1
#define CP_MAIN_2 2
#define CP_MAIN_3 3
#define CP_NOTE_1 4
#define CP_NOTE_2 5
#define CP_NOTE_3 6
#define CP_DEFAULT 7
#define CP_CURSOR 8
#define CP_SELECTION 9
#define CP_SELECTED_NOTE 10
#define CP_HL_CURSOR 11
#define CP_HL_SELECTION 12

void init_colors();
void default_window(WINDOW* win);

}
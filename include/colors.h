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
#define C_EVEN_1_BG 15
#define C_EVEN_2_BG 16
#define C_ODD_1_BG 17
#define C_ODD_2_BG 18
#define C_BAR_EDGE_BG 19
#define C_LOOP_START_BG 20
#define C_LOOP_END_BG 21

// Named colors
#define C_BG C_BW_0000
#define C_FG C_BW_0400
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
#define CP_BAR_EDGE 13
#define CP_EVEN_1 14
#define CP_EVEN_2 15
#define CP_ODD_1 16
#define CP_ODD_2 17
#define CP_EVEN_1_NOTE 18
#define CP_EVEN_2_NOTE 19
#define CP_ODD_1_NOTE 20
#define CP_ODD_2_NOTE 21
#define CP_BAR_EDGE_NOTE 22
#define CP_LOOP_START 23
#define CP_LOOP_START_NOTE 24
#define CP_LOOP_END 25
#define CP_LOOP_END_NOTE 26

void init_colors();
void default_window(WINDOW* win);

}
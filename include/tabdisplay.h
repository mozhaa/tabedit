#pragma once

#include "tab.h"
#include "global.h"
#include <ncursesw/ncurses.h>

namespace tabedit {

class TabDisplay {
private:
    Tab& tab;
    WINDOW* win;
    Global& global;

    struct point_t {
        int x;
        int y;

        bool in_between(point_t p1, point_t p2) const;
        bool operator==(point_t p) const;
    };

    point_t cursor = {0, 0};
    bool selecting = false;
    point_t selection_start = {0, 0};
    int first_bar = 0;

    void update_screen();
    unsigned int get_colorpair(point_t p) const;
    std::pair<int, int> convert_to_screen_coords(point_t p, bool& out_of_screen) const;

public:
    TabDisplay(Tab& tab, WINDOW* win, Global& global);
    void move_cursor(int dy, int dx, bool keep_selection);
    void show() const;
};

}
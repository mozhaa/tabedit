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

        point_t() = default;
        point_t(int x, int y);
        bool in_between(point_t p1, point_t p2) const;
        bool operator==(point_t p) const;
    };

    point_t cursor = {0, 0};
    point_t selection_start = {0, 0};
    int first_bar = 0;

    int mode = 0; // 0 - normal, 1 - selecting, 2 - moving
    std::vector<size_t> selection = {};

    bool save_entry_show = false;
    std::string save_entry_filename = "";

    void update_screen();
    unsigned int get_colorpair(point_t p, bool is_note) const;
    std::pair<int, int> convert_to_screen_coords(point_t p, bool& out_of_screen) const;
    void print_save_entry();
    void clear_entry();

public:
    TabDisplay(Tab& tab, WINDOW* win, Global& global);
    void move_cursor(int dy, int dx, bool keep_selection);
    void write(int n);
    void drop_selection();
    void show();
    void handle_keypress(int c);
    void show_save_entry(std::string filename);
};

}
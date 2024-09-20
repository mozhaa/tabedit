#pragma once

#include <ncursesw/ncurses.h>
#include "tab.h"

namespace tabedit {

class Global {
private:
    WINDOW* win;
    Tab& tab;

    int _maxy;
    int _maxx;
    int _width;
    int _height;
    int _winx;
    int _winy;
    int _bar_width;
    int _bar_height;
    int _note_width = 2;
    int _line_gap = 2;
    int _padding = 4;
    int _bars_per_line;
    int _lines;
    int _bars;
    int _write_timeout = 800;
    int _max_fret = 24;

public:
    Global(WINDOW* win, Tab& tab);
    void update();

    const int& maxy = _maxy;
    const int& maxx = _maxx;
    const int& width = _width;
    const int& height = _height;
    const int& winx = _winx;
    const int& winy = _winy;
    const int& bar_width = _bar_width;
    const int& bar_height = _bar_height;
    const int& note_width = _note_width;
    const int& line_gap = _line_gap;
    const int& padding = _padding;
    const int& bars_per_line = _bars_per_line;
    const int& lines = _lines;
    const int& bars = _bars;
    const int& write_timeout = _write_timeout;
    const int& max_fret = _max_fret;
};

}
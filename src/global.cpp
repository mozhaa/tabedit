#include "global.h"

namespace tabedit {

Global::Global(WINDOW* win, Tab& tab) : win(win), tab(tab) {
    update();
}

void Global::update() {
    getmaxyx(win, _maxy, _maxx);
    _bar_width = tab.dt * _note_width;
    _bar_height = tab.strings + _line_gap;

    _bars_per_line = (_maxx - _padding * 2) / _bar_width;
    _width = _bars_per_line * _bar_width;

    _lines = (_maxy - _padding * 2) / _bar_height;
    _height = _lines * _bar_height;

    _bars = _bars_per_line * _lines;

    _winx = (_maxx - _width) / 2;
    _winy = (_maxy - _height) / 2;
}

}
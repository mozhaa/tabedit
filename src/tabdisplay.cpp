#include "tabdisplay.h"

#include "colors.h"

namespace tabedit {

bool TabDisplay::point_t::in_between(point_t p1, point_t p2) const {
    return (x >= std::min(p1.x, p2.x)) && (x <= std::max(p1.x, p2.x)) && (y >= std::min(p1.y, p2.y)) && (y <= std::max(p1.y, p2.y));
}

bool TabDisplay::point_t::operator==(point_t p) const {
    return (x == p.x) && (y == p.y);
}

void TabDisplay::update_screen() {
    int current_bar = cursor.x / tab.dt;
    first_bar = std::max(std::min(first_bar, current_bar), current_bar - global.bars);
}

std::pair<int, int> TabDisplay::convert_to_screen_coords(point_t p, bool& out_of_screen) const {
    out_of_screen = false;
    int bar = p.x / tab.dt;
    if (bar < first_bar || bar > first_bar + global.bars) {
        out_of_screen = true;
        return {0, 0};
    }
    int line = (bar - first_bar) / global.bars_per_line;
    return {line * global.bar_height + p.y, ((bar - first_bar - line * global.bars_per_line) * tab.dt + p.x % tab.dt) * global.note_width};
}

void TabDisplay::move_cursor(int dy, int dx, bool keep_selection) {
    selecting = keep_selection;
    cursor.x = std::max(cursor.x + dx, 0);
    cursor.y = std::max(std::min(cursor.y + dy, tab.strings - 1), 0);
    if (!keep_selection) {
        selection_start = cursor;
    }
    update_screen();
}

unsigned int TabDisplay::get_colorpair(point_t p) const {
    if (p == cursor) {
        return COLOR_PAIR(CP_CURSOR);
    }
    if (p.in_between(cursor, selection_start)) {
        return COLOR_PAIR(CP_SELECTION);
    }
    if (p.x % tab.dt == 0) {
        return COLOR_PAIR(CP_MAIN_1);
    }
    if (p.x % 2 == 0) {
        return COLOR_PAIR(CP_MAIN_2);
    }
    if (p.x % 2 == 1) {
        return COLOR_PAIR(CP_MAIN_3);
    }
    return COLOR_PAIR(CP_DEFAULT);
}

void TabDisplay::show() const {
    for (int y = 0; y < tab.strings; ++y) {
        for (int x = first_bar * tab.dt; x < global.bars * tab.dt; ++x) {
            bool out_of_screen;
            point_t p = {x, y};
            auto [sy, sx] = convert_to_screen_coords(p, out_of_screen);
            if (out_of_screen) {
                continue;
            }
            wattron(win, get_colorpair(p));
            mvwprintw(win, sy, sx, "--");
        }
    }
    for (auto note : tab.notes) {
        bool out_of_screen;
        point_t p = {note.time, note.string};
        auto coords = convert_to_screen_coords(p, out_of_screen);
        if (out_of_screen) {
            continue;
        }
        wattron(win, get_colorpair(p));
        mvwprintw(win, coords.first, coords.second, "%*d", global.note_width, note.fret);
    }
    wattron(win, COLOR_PAIR(CP_DEFAULT) | A_BOLD);
    for (int bar = first_bar; bar < first_bar + global.bars; ++bar) {
        int lines = (bar - first_bar) / global.bars_per_line;
        int number_in_line = (bar - first_bar) % global.bars_per_line;
        int y = lines * global.bar_height + tab.strings;
        int x = number_in_line * global.bar_width;
        mvwprintw(win, y, x, "%s", std::string(global.bar_width, ' ').c_str());
        mvwprintw(win, y, x, "%d", bar + 1);
    }
    wattroff(win, A_BOLD);
}

TabDisplay::TabDisplay(Tab& tab, WINDOW* win, Global& global) : tab(tab), win(win), global(global) {
    show();
}

}
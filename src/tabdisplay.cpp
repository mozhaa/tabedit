#include "tabdisplay.h"
#include "colors.h"
#include <chrono>

namespace tabedit {

bool TabDisplay::point_t::in_between(point_t p1, point_t p2) const {
    return (x >= std::min(p1.x, p2.x)) && (x <= std::max(p1.x, p2.x)) && (y >= std::min(p1.y, p2.y)) && (y <= std::max(p1.y, p2.y));
}

bool TabDisplay::point_t::operator==(point_t p) const {
    return (x == p.x) && (y == p.y);
}

TabDisplay::point_t::point_t(int x, int y) : x(x), y(y) {}

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
    cursor.x = std::max(cursor.x + dx, 0);
    cursor.y = std::max(std::min(cursor.y + dy, tab.strings - 1), 0);
    if (mode == 2) {
        for (auto i : selection) {
            tab.notes[i].time = std::max(tab.notes[i].time + dx, 0);
            tab.notes[i].string = std::max(std::min(tab.notes[i].string + dy, tab.strings - 1), 0);
        }
    } else {
        if (!keep_selection) {
            selection_start = cursor;
            mode = 0;
        } else {
            mode = 1;
        }
    }
    update_screen();
}

void TabDisplay::write(int value) {
    static int previous_time;
    static point_t previous_coords;
    static int previous_value;
    int current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch()).count();
    if (cursor == previous_coords && current_time - previous_time <= global.write_timeout) {
        value = previous_value * 10 + value;
        if (value > global.max_fret) {
            value %= 100;
            if (value > global.max_fret) {
                value %= 10;
            }
        }
    }
    previous_time = current_time;
    previous_coords = cursor;
    previous_value = value;
    tab.set_note(Note(cursor.x, cursor.y, value));
}

unsigned int TabDisplay::get_colorpair(point_t p, bool is_note = false) const {
    if (p == cursor) {
        return COLOR_PAIR(CP_CURSOR);
    }
    if (p.in_between(cursor, selection_start) && mode == 1) {
        return COLOR_PAIR(CP_SELECTION);
    }
    if (p.x % tab.dt == 0) {
        return is_note ? COLOR_PAIR(CP_NOTE_1) : COLOR_PAIR(CP_MAIN_1);
    }
    if (p.x % 2 == 0) {
        return is_note ? COLOR_PAIR(CP_NOTE_2) : COLOR_PAIR(CP_MAIN_2);
    }
    if (p.x % 2 == 1) {
        return is_note ? COLOR_PAIR(CP_NOTE_3) : COLOR_PAIR(CP_MAIN_3);
    }
    return COLOR_PAIR(CP_DEFAULT);
}

void TabDisplay::show() {
    for (int y = 0; y < tab.strings; ++y) {
        for (int x = first_bar * tab.dt; x < global.bars * tab.dt; ++x) {
            bool out_of_screen;
            point_t p = {x, y};
            auto [sy, sx] = convert_to_screen_coords(p, out_of_screen);
            if (out_of_screen) {
                continue;
            }
            wattron(win, get_colorpair(p));
            mvwprintw(win, sy, sx, "%s", std::string(global.note_width, '-').c_str());
        }
    }
    for (auto note : tab.notes) {
        bool out_of_screen;
        point_t p = {note.time, note.string};
        auto coords = convert_to_screen_coords(p, out_of_screen);
        if (out_of_screen) {
            continue;
        }
        wattron(win, get_colorpair(p, true));
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
    wattron(win, COLOR_PAIR(CP_SELECTED_NOTE));
    for (auto i : selection) {
        auto note = tab.notes[i];
        bool out_of_screen;
        point_t p = {note.time, note.string};
        auto coords = convert_to_screen_coords(p, out_of_screen);
        if (out_of_screen) {
            continue;
        }
        mvwprintw(win, coords.first, coords.second, "%*d", global.note_width, note.fret);
    }
    wattroff(win, A_BOLD);
    if (save_entry_show) {
        print_save_entry();
    } else {
        clear_entry();
    }
}

void TabDisplay::drop_selection() {
    selection_start = cursor;
    mode = 0;
}

static char get_value (int c, bool lowercase = true) {
	const char* kn = keyname(c);
	while (*kn) {
		if (*kn >= 'A' && *kn <= 'Z') {
			return lowercase ? *kn - 'A' + 'a' : *kn;
		} else if (*kn >= 'a' && *kn <= 'z') {
			return *kn;	
		} else {
			++kn;
		}
	}
	return 0;
}

static bool is_shift(int c) {
	char k = get_value(c, false);
	return (k >= 'A') && (k <= 'Z');
}

static bool is_ctrl(int c) {
	return *keyname(c) == '^';
}

static bool is_number(int c) {
	return (c >= '0') && (c <= '9');
}

void TabDisplay::handle_keypress(int c) {
    if (is_number(c)) {
        write(c - '0');
        return;
    }
    if (c == ' ') {
        // selection action
        if (mode == 1 || mode == 0) {
            for (std::size_t i = 0; i < tab.notes.size(); ++i) {
                auto& note = tab.notes[i];
                if (point_t(note.time, note.string).in_between(cursor, selection_start)) {
                    selection.push_back(i);
                }
            }
            if (selection.size() != 0) {
                mode = 2;
                selection_start = cursor;
            }
        } else if (mode == 2) {
            tab.overwrite_by_selected(selection);
            selection = {};
            selection_start = cursor;
            mode = 0;
        }
        return;
    }
    char v = get_value(c);
    switch(v) {
    case 'a':
        if (is_ctrl(c)) {
            move_cursor(0, -tab.dt, is_shift(c));
        } else {
            move_cursor(0, -1, is_shift(c));
        }
        break;
    case 'd':
        if (is_ctrl(c)) {
            move_cursor(0, tab.dt, is_shift(c));
        } else {
            move_cursor(0, 1, is_shift(c));
        }
        break;
    case 'w':
        if (is_ctrl(c)) {
            move_cursor(0, -tab.dt * global.bars_per_line, is_shift(c));
        } else {
            move_cursor(-1, 0, is_shift(c));
        }
        break;
    case 's':
        if (is_ctrl(c)) {
            move_cursor(0, tab.dt * global.bars_per_line, is_shift(c));
        } else {
            move_cursor(1, 0, is_shift(c));
        }
        break;
    case 'r':
        if (mode == 0) {
            tab.delete_on(cursor.x, cursor.y);
        } else if (mode == 2) {
            tab.delete_selected(selection);
            selection = {};
            selection_start = cursor;
            mode = 0;
        }
        break;
    }
}

void TabDisplay::clear_entry() {
    wattron(win, COLOR_PAIR(CP_DEFAULT));
    mvwprintw(win, global.height - 1, 0, "%s", std::string(global.width, ' ').c_str());
}

void TabDisplay::print_save_entry() {
    save_entry_show = false;
    wattron(win, A_BOLD);
    mvwprintw(win, global.height - 1, 0, "Tab had been saved into %s", save_entry_filename.c_str());
    wattroff(win, A_BOLD);
}

void TabDisplay::show_save_entry(std::string filename) {
    save_entry_show = true;
    save_entry_filename = filename;
}

TabDisplay::TabDisplay(Tab& tab, WINDOW* win, Global& global) : tab(tab), win(win), global(global) {
    show();
}

}
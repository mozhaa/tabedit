#include "editor.h"
#include "tab.h"
#include "tabdisplay.h"
#include "keybindings.h"
#include "json.hpp"
#include "global.h"
#include "colors.h"
#include <fstream>

using json = nlohmann::json;

namespace tabedit {

static Tab from_file(std::string filename) {
    std::ifstream f(filename);
    if (f.fail()) {
        throw std::runtime_error("No such file: " + filename);
    }
    json data = json::parse(f);
    return Tab(data);
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

void run(std::string filename) {
    Tab tab = from_file(filename);
	initscr();
    raw();
    noecho();
	cbreak();
	curs_set(0);
	init_colors();
	Global global(stdscr, tab);
	WINDOW* win = newwin(global.height, global.width, global.winy, global.winx);
	default_window(win);
	TabDisplay display(tab, win, global);
	refresh();

	bool running = true;
	while (running) {
		display.show();
		int c = wgetch(win);
		int v = get_value(c);
		switch (v) {
		case K_EXIT:
			running = false;
			break;
		case K_LEFT:
			if (is_ctrl(c)) {
				display.move_cursor(0, -tab.dt, is_shift(c));
			} else {
				display.move_cursor(0, -1, is_shift(c));
			}
			break;
		case K_RIGHT:
			if (is_ctrl(c)) {
				display.move_cursor(0, tab.dt, is_shift(c));
			} else {
				display.move_cursor(0, 1, is_shift(c));
			}
			break;
		case K_UP:
			if (is_ctrl(c)) {
				display.move_cursor(0, -tab.dt * global.bars_per_line, is_shift(c));
			} else {
				display.move_cursor(-1, 0, is_shift(c));
			}
			break;
		case K_DOWN:
			if (is_ctrl(c)) {
				display.move_cursor(0, tab.dt * global.bars_per_line, is_shift(c));
			} else {
				display.move_cursor(1, 0, is_shift(c));
			}
			break;
		}
	}
	endwin();
}

}
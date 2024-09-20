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

void run(std::string filename) {
    Tab tab(filename);
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
		std::string new_filename;
		switch (c) {
		case K_EXIT:
			running = false;
			break;
		case 'e':
			// save
			display.show_save_entry(tab.save());
			break;
		case 'f':
			new_filename = display.get_new_filename();
			tab.fork(new_filename);
			display.show_save_entry(new_filename);
			break;
		default:
			display.handle_keypress(c);
			break;
		}
	}
	endwin();
}

}
#include "editor.h"
#include "tab.h"
#include <nlohmann/json.hpp>

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

void run(std::string filename) {
    Tab tab = from_file(filename);
    initscr();
    raw();
    noecho();
    printw("Type any character to see it in bold\n");
	int ch = getch();			/* If raw() hadn't been called
					 * we have to press enter before it
					 * gets to the program 		*/
	if(ch == KEY_F(1))		/* Without keypad enabled this will */
		printw("F1 Key pressed");/*  not get to us either	*/
					/* Without noecho() some ugly escape
					 * charachters might have been printed
					 * on screen			*/
	else
	{	printw("The pressed key is ");
		attron(A_BOLD);
		printw("%c", ch);
		attroff(A_BOLD);
	}
	refresh();			/* Print it on to the real screen */
    	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */
}

}
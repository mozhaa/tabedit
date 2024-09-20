#pragma once

#include <vector>
#include <math.h>
#include <json.hpp>

using json = nlohmann::json;

namespace tabedit {

struct Note {
    int time;
    int string;
    int fret;

    Note(int time, int string, int fret) : time(time), string(string), fret(fret) {}
};

class Tab {
public:
    int strings;
    int dt;
    std::vector<Note> notes;
    std::string filename;

    void set_note(Note n);
    void overwrite_by_selected(std::vector<size_t> selection);
    void delete_on(int x, int y);
    void delete_selected(std::vector<size_t> selection);
    void copy_selected(std::vector<size_t> selection);
    std::string save() const;
    void fork(std::string new_filename);
    Tab(std::string filename);
    Tab(int strings = 6, int dt = 16) : strings(strings), dt(dt), notes({}) {}

private:
};

}
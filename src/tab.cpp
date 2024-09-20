#include "tab.h"
#include <fstream>

namespace tabedit {

// int Fraction::count(int dt) {
//     return floor((float)num * dt / denom);
// }

void Tab::overwrite_by_selected(std::vector<size_t> selection) {
    std::vector<Note> new_notes = {};
    for (auto i : selection) {
        auto selected_note = notes[i];
        new_notes.push_back(selected_note);
    }
    for (auto note : notes) {
        bool add = true;
        for (auto i : selection) {
            auto selected_note = notes[i];
            if (selected_note.string == note.string && selected_note.time == note.time) {
                add = false;
                break;
            }
        }
        if (add) {
            new_notes.push_back(note);
        }
    }
    notes = new_notes;
}

void Tab::delete_on(int x, int y) {
    std::vector<Note> new_notes = {};
    for (std::size_t i = 0; i < notes.size(); ++i) {
        if (!(notes[i].time == x && notes[i].string == y)) {
            new_notes.push_back(notes[i]);
        }
    }
    notes = new_notes;
}

void Tab::delete_selected(std::vector<size_t> selection) {
    std::vector<Note> new_notes = {};
    for (std::size_t i = 0; i < notes.size(); ++i) {
        bool add = true;
        for (auto j : selection) {
            if (i == j) {
                add = false;
                break;
            }
        }
        if (add) {
            new_notes.push_back(notes[i]);
        }
    }
    notes = new_notes;
}

void Tab::set_note(Note n) {
    for (auto& note : notes) {
        if (note.string == n.string && note.time == n.time) {
            note.fret = n.fret;
            return;
        }
    }
    notes.push_back(n);
}

Tab::Tab(std::string filename) : filename(filename) {
    std::ifstream f(filename);
    if (f.fail()) {
        throw std::runtime_error("No such file: " + filename);
    }
    json data = json::parse(f);
    strings = data["strings"];
    dt = data["dt"];
    notes = {};
    for (auto note : data["notes"]) {
        notes.push_back(Note(note["time"], note["string"], note["fret"]));
    }
}

std::string Tab::save() const {
    json data;
    data["strings"] = strings;
    data["dt"] = dt;
    std::vector<json> _notes = {};
    for (auto note : notes) {
        json _note;
        _note["time"] = note.time;
        _note["string"] = note.string;
        _note["fret"] = note.fret;
        _notes.push_back(_note);
    }
    data["notes"] = _notes;
    std::ofstream f(filename);
    if (f.fail()) {
        f = std::ofstream("backup.json");
    }
    f << data.dump();
    return filename;
}

}
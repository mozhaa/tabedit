#include "tab.h"
#include <fstream>
#include <map>

namespace tabedit {

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

void Tab::copy_selected(std::vector<size_t> selection) {
    std::vector<Note> copied_notes = {};
    for (auto i : selection) {
        copied_notes.push_back(notes[i]);
    }
    notes.insert(notes.end(), copied_notes.begin(), copied_notes.end());
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

static std::map<std::string, int> letters = {
    {"A", -3},
    {"A#", -2},
    {"Bb", -2},
    {"B", -1},
    {"C", 0},
    {"C#", 1},
    {"Db", 1},
    {"D", 2},
    {"D#", 3},
    {"Eb", 3},
    {"E", 4},
    {"F", 5},
    {"F#", 6},
    {"Gb", 6},
    {"G", 7},
    {"G#", 8},
    {"Ab", 8}
};

Tab::Tab(std::string filename) : filename(filename) {
    std::ifstream f(filename);
    if (f.fail()) {
        throw std::runtime_error("No such file: " + filename);
    }
    json data = json::parse(f);
    strings = data["strings"];
    dt = data["dt"];
    bpm = data["bpm"];
    tuning = std::vector<int>(strings, 0);
    for (int i = 0; i < strings; ++i) {
        std::string letter = data["tuning"][i]["letter"];
        int number = data["tuning"][i]["number"];
        tuning[i] = letters[letter] + number * 12;
    }
    notes = {};
    for (auto note : data["notes"]) {
        notes.push_back(Note(note["time"], note["string"], note["fret"]));
    }
}

std::string Tab::save() const {
    json data;
    data["strings"] = strings;
    data["dt"] = dt;
    data["bpm"] = bpm;
    std::vector<json> _tuning = {};
    for (int i = 0; i < strings; ++i) {
        int number = -1;
        std::string letter;
        int value = tuning[i];
        bool found = false;
        while (!found && number < 10) {
            for (auto& [_letter, _number] : letters) {
                if (_number == value) {
                    letter = _letter;
                    found = true;
                    break;
                }
            }
            ++number;
            value -= 12;
        }
        if (!found) {
            number = 5;
            letter = "C";
        }
        _tuning.push_back({{"letter", letter}, {"number", number}});
    }
    data["tuning"] = _tuning;
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

void Tab::fork(std::string new_filename) {
    filename = new_filename;
    save();
}

}
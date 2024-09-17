#include "tab.h"

namespace tabedit {

int Fraction::count(int dt) {
    return floor((float)num * dt / denom);
}

Tab::Tab(json data) {
    strings_num = data["strings_num"];
    dt = data["dt"];
    notes = {};
    for (auto note : data["notes"]) {
        notes.push_back(Note(Fraction(note["time"]["num"], note["time"]["denom"]), note["string"], note["fret"]));
    }
}

}
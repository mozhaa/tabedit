#include "tab.h"

namespace tabedit {

// int Fraction::count(int dt) {
//     return floor((float)num * dt / denom);
// }

Tab::Tab(json data) {
    strings = data["strings"];
    dt = data["dt"];
    notes = {};
    for (auto note : data["notes"]) {
        notes.push_back(Note(note["time"], note["string"], note["fret"]));
    }
}

}
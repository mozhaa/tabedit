#pragma once

#include <vector>
#include <math.h>
#include <json.hpp>

using json = nlohmann::json;

namespace tabedit {

// struct Fraction {
//     int num;
//     int denom;

//     Fraction(int num, int denom) : num(num), denom(denom) {}

//     int count(int dt);

//     // TODO: operator+-
// };

struct Note {
    // Fraction time;
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

    Tab(int strings = 6, int dt = 16) : strings(strings), dt(dt), notes({}) {}
    Tab(json data);
};

}
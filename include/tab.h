#pragma once

#include <vector>
#include <math.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace tabedit {

struct Fraction {
    int num;
    int denom;

    Fraction(int num, int denom) : num(num), denom(denom) {}

    int count(int dt);

    // TODO: operator+-
};

struct Note {
    Fraction time;
    int string;
    int fret;

    Note(Fraction time, int string, int fret) : time(time), string(string), fret(fret) {}
};

class Tab {
private:
    int strings_num;
    int dt;

public:
    std::vector<Note> notes;

    Tab(int strings_num = 6, int dt = 16) : strings_num(strings_num), dt(dt), notes({}) {}
    Tab(json data);
};

}
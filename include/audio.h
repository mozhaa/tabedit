#pragma once

#include "tab.h"

#include <SFML/Audio.hpp>
#include <thread>
#include <vector>
#include <atomic>
#include <fstream>

namespace tabedit {

#define SAMPLE_RATE 44100
#define SECONDS 2
#define MILLISECONDS (1000*(SECONDS))
#define LENGTH ((SECONDS)*(SAMPLE_RATE))
#define RANGE 128

class TabPlayer {
    std::vector<std::vector<short>> samples;

    int strings;
    std::vector<std::thread*> threads;
    
    std::vector<std::vector<Note>> notes;

    std::atomic_bool playing = false;
    
    // debug
    std::ofstream debug;
    
    void play_string(int string, int min_time, int max_time, int tuning, int dt, int bpm);

public:
    TabPlayer(std::string soundfont_filename, int strings);
    void start(std::vector<Note> _notes, std::vector<int> tuning, int dt, int bpm, int min_time = 0, int max_time = 1000000);
    void stop();
};

}
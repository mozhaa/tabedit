#pragma once

#include "tab.h"
#include "tabdisplay.h"

#include <SFML/Audio.hpp>
#include <thread>
#include <vector>
#include <atomic>

namespace tabedit {

#define SAMPLE_RATE 44100
#define SECONDS 2
#define MILLISECONDS (1000*(SECONDS))
#define LENGTH ((SECONDS)*(SAMPLE_RATE))
#define RANGE 128

class TabPlayer {
    std::vector<std::vector<short>> samples;

    int strings;
    std::vector<sf::Sound> sounds;
    std::vector<sf::SoundBuffer> buffers;

    std::atomic_bool playing = false;
    std::thread* main_thread;

public:
    TabPlayer(std::string soundfont_filename, int strings);
    void start(std::vector<Note> notes, std::vector<int> tuning, int dt, float bpm, int min_time, int max_time, TabDisplay* display);
    void stop();
};

}
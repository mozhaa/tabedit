#include "audio.h"

#include <algorithm>
#include <chrono>
#include <cstdio>

namespace tabedit {

#define sleep(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))

TabPlayer::TabPlayer(std::string samples_filename, int strings) : strings(strings) {
    // read samples from binary file
    samples = std::vector<std::vector<sf::Int16>>(RANGE, std::vector<sf::Int16>(LENGTH, 0));
    FILE* samples_file = fopen(samples_filename.c_str(), "rb");
    if (!samples_file) {
        throw std::runtime_error("Error opening samples file " + samples_filename);
    }
    for (std::size_t i = 0; i < RANGE; ++i) {
        if (fread(samples[i].data(), sizeof(short), LENGTH, samples_file) != LENGTH) {
            throw std::runtime_error("Error reading from samples file");
        }
    }
    fclose(samples_file);
}

void TabPlayer::start(std::vector<Note> notes, std::vector<int> tuning, int dt, float bpm, int min_time, int max_time, TabDisplay* display) {
    if (playing) {
        stop();
    }
    std::sort(notes.begin(), notes.end(), [](const Note& x, const Note& y){
        return x.time <= y.time;
    });
    playing = true;
    main_thread = new std::thread([this, notes, tuning, dt, bpm, min_time, max_time, display](){
        std::vector<sf::Sound> sounds = std::vector<sf::Sound>(strings, sf::Sound());
        std::vector<sf::SoundBuffer> buffers = std::vector<sf::SoundBuffer>(strings, sf::SoundBuffer());
        auto note = notes.begin();
        const int dt_ms = 4 * 60000.f / bpm / dt;
        for (int time = min_time; time <= max_time; ++time) {
            if (!playing) {
                // stop everything
                for (int i = 0; i < strings; ++i) {
                    sounds[i].stop();
                }
                break;
            }
            while(note != notes.end() && note->time < time) {
                // find next note
                ++note;
            }
            if (note == notes.end()) {
                // or we'd go into infinity
                break;
            }
            if (note->time == time) {
                // play note
                sounds[note->string].stop();
                buffers[note->string].loadFromSamples(samples[tuning[note->string] + note->fret].data(), LENGTH, 1, SAMPLE_RATE);
                sounds[note->string].setBuffer(buffers[note->string]);
                sounds[note->string].play();
            }
            display->update_play_cursor(time);
            sleep(dt_ms);
        }
        playing = false;
        display->update_play_cursor(-1);       
    });
}

void TabPlayer::stop() {
    if (!playing) {
        return;
    }
    playing = false;
    main_thread->join();
    delete main_thread;
}

}
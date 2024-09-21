#include "audio.h"

#include <algorithm>
#include <chrono>
#include <cstdio>

namespace tabedit {

#define sleep(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))

static int to_ms(int time, int dt, float bpm) {
    return 4 * 60000.f * time / bpm / dt;
}

void TabPlayer::play_string(int string, int min_time, int max_time, int tuning, int dt, float bpm) {
    if (notes[string].size() == 0) {
        return;
    }
    sleep(to_ms(notes[string][0].time - min_time, dt, bpm));
    for (std::size_t i = 0; i < notes[string].size() && playing; ++i) {
        sf::Sound sound;
        sf::SoundBuffer buffer;
        auto note = notes[string][i];
        buffer.loadFromSamples(samples[tuning + note.fret].data(), LENGTH, 1, SAMPLE_RATE);
        sound.setBuffer(buffer);
        sound.play();
        if (i != notes[string].size() - 1) {
            // wait till next note
            sleep(to_ms(notes[string][i + 1].time - notes[string][i].time, dt, bpm));
        } else {
            // wait full note length
            sleep(MILLISECONDS);
        }
        sound.stop();
    }
}

TabPlayer::TabPlayer(std::string samples_filename, int strings) : strings(strings), threads(strings, nullptr) {
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

void TabPlayer::start(std::vector<Note> _notes, std::vector<int> tuning, int dt, float bpm, int min_time, int max_time) {
    if (playing) {
        stop();
    }
    notes = std::vector<std::vector<Note>>(strings, std::vector<Note>());
    for (auto note : _notes) {
        if (note.time <= max_time && note.time >= min_time) {
            notes[note.string].push_back(note);
        }
    }
    for (int i = 0; i < strings; ++i) {
        std::sort(notes[i].begin(), notes[i].end(), [](const Note& x, const Note& y){
            return x.time <= y.time;
        });
    }
    playing = true;
    for (int i = 0; i < strings; ++i) {
        threads[i] = new std::thread([this, i, min_time, max_time, &tuning, dt, bpm](){
            return this->play_string(i, min_time, max_time, tuning[i], dt, bpm);
        });
    }
}

void TabPlayer::stop() {
    if (!playing) {
        return;
    }
    playing = false;
    for (int i = 0; i < strings; ++i) {
        threads[i]->join();
        delete threads[i];
    }
    notes.clear();
}

}
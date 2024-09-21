#include "audio.h"

#include <algorithm>
#include <chrono>
#include <cstdio>

namespace tabedit {

static int get_milliseconds(int time, int dt, float bpm) {
    return 4 * 60000.f * time / bpm / dt;
}

static void sleep(int time, int dt, float bpm, std::ofstream& debug, int string) {
    if (string == 1) debug << "sleeping " << get_milliseconds(time, dt, bpm) << "ms" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(get_milliseconds(time, dt, bpm)));
}

void TabPlayer::play_string(int string, int min_time, int max_time, int tuning, int dt, float bpm) {
    if (string == 1) debug << "Thread #" << string << ": play_string()" << std::endl;
    if (notes[string].size() == 0) {
        return;
    }
    if (string == 1) debug << "Thread #" << string << ": note " << notes[string][0].string << ", " << notes[string][0].time << ", " << notes[string][0].fret << std::endl;
    sleep(notes[string][0].time - min_time, dt, bpm, debug, string);
    if (string == 1) debug << "Thread #" << string << ": end sleeping" << std::endl;
    for (std::size_t i = 0; i < notes[string].size() && playing; ++i) {
        sf::Sound sound;
        sf::SoundBuffer buffer;
        auto note = notes[string][i];
        if (string == 1) debug << "Thread #" << string << ": note " << note.string << ", " << note.time << ", " << note.fret << std::endl;
        buffer.loadFromSamples(samples[tuning + note.fret].data(), LENGTH, 1, SAMPLE_RATE);
        sound.setBuffer(buffer);
        sound.play();
        sleep(i != notes[string].size() - 1 ? notes[string][i + 1].time - notes[string][i].time : MILLISECONDS, dt, bpm, debug, string);
        sound.stop();
    }
}

TabPlayer::TabPlayer(std::string samples_filename, int strings) : strings(strings), threads(strings, nullptr) {
    debug = std::ofstream("debug.log");
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
    debug << "start()" << std::endl;
    if (playing) {
        stop();
    }
    notes = std::vector<std::vector<Note>>(strings, std::vector<Note>());
    for (auto note : _notes) {
        if (note.time <= max_time && note.time >= min_time) {
            notes[note.string].push_back(note);
        }
    }
    debug << "Separated notes into vectors" << std::endl;
    for (int i = 0; i < strings; ++i) {
        std::sort(notes[i].begin(), notes[i].end(), [](const Note& x, const Note& y){
            return x.time <= y.time;
        });
    }
    debug << "Sorted all vectors" << std::endl;
    debug << tuning[0] << " " << tuning[1] << " " << tuning[2] << " " << tuning[3] << std::endl;
    playing = true;
    for (int i = 0; i < strings; ++i) {
        debug << "Starting thread #" << i << std::endl;
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
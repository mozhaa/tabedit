#include "audio.h"

#include <algorithm>
#include <chrono>
#include "tsf.h"

namespace tabedit {

static int get_milliseconds(int time, int dt, int bpm) {
    return 60000.f * time / bpm / dt;
}

static void sleep(int time, int dt, int bpm, std::ofstream& debug, int string) {
    if (string == 1) debug << "sleeping " << get_milliseconds(time, dt, bpm) << "ms" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(get_milliseconds(time, dt, bpm)));
}

void TabPlayer::play_string(int string, int min_time, int max_time, int tuning, int dt, int bpm) {
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

TabPlayer::TabPlayer(std::string soundfont_filename, int strings) : strings(strings), threads(strings, nullptr) {
    debug = std::ofstream("debug.log");
    tsf* soundfont = tsf_load_filename(soundfont_filename.c_str());
    tsf_set_output(soundfont, TSF_MONO, SAMPLE_RATE, 0);
    // pre-generate samples for whole note range
    debug << "starting generating" << std::endl;
    samples = std::vector<std::vector<sf::Int16>>(RANGE, std::vector<sf::Int16>(LENGTH, 0));
    for (int i = 0; i < RANGE; ++i) {
        tsf_note_on(soundfont, 0, i, 1.0f);
        tsf_render_short(soundfont, samples[i].data(), LENGTH, 0);
    }
    debug << "ehm" << std::endl;
    sf::Sound sound;
    debug << "ad" << std::endl;
    sf::SoundBuffer buffer;
    debug << "start" << std::endl;
    buffer.loadFromSamples(samples[60].data(), LENGTH, 1, SAMPLE_RATE);
    debug << "lodade from samples" << std::endl;
    sound.setBuffer(buffer);
    debug << "set buffer" << std::endl;
    sound.play();
    debug << "sleeping 2000 ms" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(MILLISECONDS));
    sound.stop();
    debug << "Generated samples for whole note range" << std::endl;
}

void TabPlayer::start(std::vector<Note> _notes, std::vector<int> tuning, int dt, int bpm, int min_time, int max_time) {
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
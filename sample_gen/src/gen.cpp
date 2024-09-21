#include "tsf.h"
#include <cstdio>

#define RANGE 128
#define SAMPLE_RATE 44100
#define SECONDS 2
#define MILLISECONDS (1000*(SECONDS))
#define LENGTH ((SECONDS)*(SAMPLE_RATE))

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: ./gen <sf2 input> <bin output>\n");
        return 1;
    }
    char* input_filename = argv[1];
    char* output_filename = argv[2];
    FILE* output_file = fopen(output_filename, "wb");
    tsf* soundfont = tsf_load_filename(input_filename);
    tsf_set_output(soundfont, TSF_MONO, SAMPLE_RATE, 0);
    short buffer[LENGTH];
    for (int i = 0; i < RANGE; ++i) {
        tsf_note_on(soundfont, 0, i, 1.0f);
        tsf_render_short(soundfont, buffer, LENGTH, 0);
        int written;
        if ((written = fwrite(buffer, sizeof(short), LENGTH, output_file)) != LENGTH) {
            printf("Error during write: expected %d, actually written %d\n", LENGTH, written);
            fclose(output_file);
            return -1;
        }
    }
    fclose(output_file);
    return 0;
}
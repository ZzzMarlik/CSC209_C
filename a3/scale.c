#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
int scale_factor;

/*
 * Main filter loop.
 * This function is responsible for doing the following:
 *   1. Read in pixels one at a time (because copy is a pixel-by-pixel transformation).
 *   2. Immediately write out each pixel.
 *
 * Note that this function should allocate space only for a single Pixel;
 * do *not* store more than one Pixel at a time, it isn't necessary here!
 */
void scale_filter(Bitmap *bmp) {
    // TODO: Complete this function.
    Pixel odd_pixels[bmp->height][bmp->width];
    for (int i = 0; i < bmp->height; i++) {
        for (int j = 0; j < bmp->width; j++) {
            if (fread(&(odd_pixels[i][j]), sizeof(Pixel), 1, stdin) != 1) {
                fprintf(stderr, "Error: info of pixel could not be read.\n");
            }
        }
    }
    for (int a = 0; a < (bmp->height * scale_factor); a++) {
        for (int b = 0; b < (bmp->width * scale_factor); b++) {
            if (fwrite(&(odd_pixels[a / scale_factor][b / scale_factor]), sizeof(Pixel), 1, stdout) != 1) {
                fprintf(stderr, "Error: info of pixel could not be read.\n");
            }
        }
    }
}

int main(int argc, char **argv) {
    // Run the filter program with copy_filter to process the pixels.
    // You shouldn't need to change this implementation.
    if (argc < 2) {
        printf("Invalid input for the scale filter\n");
        exit(1);
    }
    scale_factor = strtol(argv[1], NULL, 10);
    run_filter(scale_filter, scale_factor);
    return 0;
}
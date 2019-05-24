#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"


/*
 * Main filter loop.
 * This function is responsible for doing the following:
 *   1. Read in pixels one at a time (because copy is a pixel-by-pixel transformation).
 *   2. Immediately write out each pixel.
 *
 * Note that this function should allocate space only for a single Pixel;
 * do *not* store more than one Pixel at a time, it isn't necessary here!
 */
void copy_filter(Bitmap *bmp) {
    // TODO: Complete this function.
    int file_size = bmp->width * bmp->height;
    for (int i = 0; i < file_size; i++) {
        Pixel *p = malloc(sizeof(Pixel));
        int read = fread(p, sizeof(Pixel), 1, stdin);
        if (read != 1) {
            fprintf(stderr, "Error: info of pixel could not be read.\n");
        }
        int error = fwrite(p, sizeof(Pixel), 1, stdout);
        if (error != 1) {
            fprintf(stderr, "Error: info of pixel could not be read.\n");
        }
    }
}

int main() {
    // Run the filter program with copy_filter to process the pixels.
    // You shouldn't need to change this implementation.
    run_filter(copy_filter, 1);
    return 0;
}
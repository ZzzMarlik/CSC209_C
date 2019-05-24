#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

void greyscale_filter(Bitmap *bmp) {
    // TODO: Complete this function.
    int total_pixels = bmp->width * bmp->height;
    for (int i = 0; i < total_pixels; i++) {
        Pixel *p = malloc(sizeof(Pixel));
        int read = fread(p, sizeof(Pixel), 1, stdin);
        if (read != 1) {
            fprintf(stderr, "Error: info of pixel could not be read.\n");
        }
        float new_color = (p->blue + p->green + p->red) / 3;
        p->green = new_color;
        p->red = new_color;
        p->blue = new_color;
        int error = fwrite(p, sizeof(Pixel), 1, stdout);
        if (error != 1) {
            fprintf(stderr, "Error: info of pixel could not be write.\n");
        }
    }
}

int main() {
    // Run the filter program with copy_filter to process the pixels.
    // You shouldn't need to change this implementation.
    run_filter(greyscale_filter, 1);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

void edge_detection_filter(Bitmap *bmp) {
    int width = bmp->width;
    Pixel top[width];
    Pixel mid[width];
    Pixel bot[width];
    int error_1 = fread(top, sizeof(Pixel), bmp->width, stdin);
    int error_2 = fread(mid, sizeof(Pixel), bmp->width, stdin);
    int error_3 = fread(bot, sizeof(Pixel), bmp->width, stdin);
    if (error_1 != bmp->width || error_2 != bmp->width || error_3 != bmp->width) {
        fprintf(stderr, "Error: info of pixel could not be read.\n");
    }
    int row = 0;
    int col = 0;
    Pixel new_ps[width];
    while (col < bmp->width - 1) { // handle the first row and second row
        if (col == 0) { // handle (0, 0)
            Pixel new_p = apply_edge_detection_kernel(&(top[col]), &(mid[col]), &(bot[col]));// get new (1, 1)
            new_ps[col] = new_p;
            new_ps[col + 1] = new_p;
            col += 2;
            int e1, e2;
            e1 = fwrite(&new_p, sizeof(Pixel), 1, stdout); // write out new (0, 0)
            e2 = fwrite(&new_p, sizeof(Pixel), 1, stdout); // write out new (0, 1)
            if (e1 != 1 || e2 != 1) {
                fprintf(stderr, "Error: info of pixel could not be write.\n");
            }
        } else if (col == bmp->width - 2) { // handle (0, width - 2) and (0, width - 1)
            Pixel new_p = apply_edge_detection_kernel(&(top[col - 1]), &(mid[col - 1]), &(bot[col - 1])); //get new (1, width - 2)
            new_ps[col] = new_p;
            new_ps[col + 1] = new_p;
            col += 2;
            int e1, e2;
            e1 = fwrite(&new_p, sizeof(Pixel), 1, stdout); // write out new (0, 0)
            e2 = fwrite(&new_p, sizeof(Pixel), 1, stdout); // write out new (0, 1)
            if (e1 != 1 || e2 != 1) {
                fprintf(stderr, "Error: info of pixel could not be write.\n");
            }
        } else { // middle cases
            Pixel new_p = apply_edge_detection_kernel(&(top[col - 1]), &(mid[col - 1]), &(bot[col - 1]));
            new_ps[col] = new_p;
            col += 1;
            int e1 = fwrite(&new_p, sizeof(Pixel), 1, stdout);
            if (e1 != 1) {
                fprintf(stderr, "Error: info of pixel could not be write.\n");
            }
        }
    }
    for (int x = 0; x < bmp->width; x++) { // write the mid pixels
        int error = fwrite(&(new_ps[x]), sizeof(Pixel), 1, stdout);
        if (error != 1) {
            fprintf(stderr, "Error: info of pixel could not be write.\n");
        }
    }
    row += 2; // change to the next mid row
    col = 0;
    while (row < bmp->height - 2){ // middle cases
        col = 0;
        for (int y = 0; y < width; y++) {
            top[y] = mid[y];
            mid[y] = bot[y];
        }
        int error_4 = fread(bot, sizeof(Pixel), bmp->width, stdin); // read new row of pixels
        if (error_4 != bmp->width) {
            fprintf(stderr, "Error: info of pixel could not be read.\n");
        }
        while (col < bmp->width - 1) {
            if (col == 0) {
                Pixel new_p = apply_edge_detection_kernel(&(top[col]), &(mid[col]), &(bot[col]));
                col += 2;
                int e1, e2;
                e1 = fwrite(&new_p, sizeof(Pixel), 1, stdout);
                e2 = fwrite(&new_p, sizeof(Pixel), 1, stdout);
                if (e1 != 1 || e2 != 1) {
                    fprintf(stderr, "Error: info of pixel could not be write.\n");
                }
            } else if (col == bmp->width - 2) {
                Pixel new_p = apply_edge_detection_kernel(&(top[col - 1]), &(mid[col - 1]), &(bot[col - 1]));
                col += 2;
                int e1, e2;
                e1 = fwrite(&new_p, sizeof(Pixel), 1, stdout); // write out new (0, 0)
                e2 = fwrite(&new_p, sizeof(Pixel), 1, stdout); // write out new (0, 1)
                if (e1 != 1 || e2 != 1) {
                    fprintf(stderr, "Error: info of pixel could not be write.\n");
                }
            } else { // middle cases
                Pixel new_p = apply_edge_detection_kernel(&(top[col - 1]), &(mid[col - 1]), &(bot[col - 1]));
                col += 1;
                int e1 = fwrite(&new_p, sizeof(Pixel), 1, stdout);
                if (e1 != 1) {
                    fprintf(stderr, "Error: info of pixel could not be write.\n");
                }
            }
        }
        row += 1; // move to the next row.
    }
    //handle the last two rows
    for (int y = 0; y < width; y++) {
        top[y] = mid[y];
        mid[y] = bot[y];
    }
    col = 0;
    int error_4 = fread(bot, sizeof(Pixel), bmp->width, stdin); // read new row of pixels
    if (error_4 != bmp->width) {
        fprintf(stderr, "Error: info of pixel could not be read.\n");
    }
    while (col < bmp->width - 1) { // handle the last row and second last row
        if (col == 0) {
            Pixel new_p = apply_edge_detection_kernel(&(top[col]), &(mid[col]), &(bot[col]));
            new_ps[col] = new_p;
            new_ps[col + 1] = new_p;
            col += 2;
            int e1, e2;
            e1 = fwrite(&new_p, sizeof(Pixel), 1, stdout); // write out new (0, 0)
            e2 = fwrite(&new_p, sizeof(Pixel), 1, stdout); // write out new (0, 1)
            if (e1 != 1 || e2 != 1) {
                fprintf(stderr, "Error: info of pixel could not be write.\n");
            }
        } else if (col == bmp->width - 2) {
            Pixel new_p = apply_edge_detection_kernel(&(top[col - 1]), &(mid[col - 1]), &(bot[col - 1]));
            new_ps[col] = new_p;
            new_ps[col + 1] = new_p;
            col += 2;
            int e1, e2;
            e1 = fwrite(&new_p, sizeof(Pixel), 1, stdout); // write out new (0, 0)
            e2 = fwrite(&new_p, sizeof(Pixel), 1, stdout); // write out new (0, 1)
            if (e1 != 1 || e2 != 1) {
                fprintf(stderr, "Error: info of pixel could not be write.\n");
            }
        } else { // middle cases
            Pixel new_p = apply_edge_detection_kernel(&(top[col - 1]), &(mid[col - 1]), &(bot[col - 1]));
            new_ps[col] = new_p;
            col += 1;
            int e1 = fwrite(&new_p, sizeof(Pixel), 1, stdout);
            if (e1 != 1) {
                fprintf(stderr, "Error: info of pixel could not be write.\n");
            }
        }
    }
    for (int x = 0; x < bmp->width; x++) { // write the last row pixels
        int error = fwrite(&(new_ps[x]), sizeof(Pixel), 1, stdout);
        if (error != 1) {
            fprintf(stderr, "Error: info of pixel could not be write.\n");
        }
    }
}

int main() {
    // Run the filter program with copy_filter to process the pixels.
    // You shouldn't need to change this implementation.
    run_filter(edge_detection_filter, 1);
    return 0;
}
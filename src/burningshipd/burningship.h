#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define _LIMIT 4 //our convergence/divergence threshold

void burning_ship(float complex start, size_t width, size_t height, float res, unsigned n, unsigned char *img);

unsigned iterate_pixel(float pixel_x, float pixel_y, unsigned n);

void painter(unsigned iteration_to_escape, unsigned char *img, unsigned img_index, unsigned n);
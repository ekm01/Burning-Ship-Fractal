#include <immintrin.h>
#include <complex.h>

__m128 vectorialAbs(__m128 a);

unsigned isAllZero(__m128i a);

__m128i iterate_pixel_p(__m128 pixels_x, __m128 pixels_y, __m128i v_n);

void painter_p(__m128i iteration_to_escape, unsigned char *img, unsigned img_index, unsigned n);

void burning_ship_p(float complex start, size_t width, size_t height, float res, unsigned n, unsigned char *img);

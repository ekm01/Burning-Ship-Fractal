#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <immintrin.h>
#include "burningship.h"

__m128 vectorialAbs(__m128 a){
    __m128 absConstant = _mm_set1_ps(-0.0); //binary for this is 1000000...
    //what the functions does is NEG the first parameter and AND with the second parameter, so:
    //NEG 1000000... = 0111111...
    //0111111... AND 1101011... = 0101011... => which is the abs value
    return _mm_andnot_ps(absConstant,a);
}

unsigned isAllZero(__m128i a){ //checks if the vector is a zero vector
    for (size_t i = 0; i < 4; i++)
    {
        int32_t* element = (int32_t*) &a;
        if (element[i] == 1)
        {
            return 0;
        }
    }
    return 1;
}

__m128i iterate_pixel_p(__m128 pixels_x, __m128 pixels_y, __m128i v_n) // Check if the coordinates are either converge and diverge
{
    __m128 LIMIT = _mm_set1_ps(4.0); // Our convergence/divergence threshold
    __m128 eqConstant = _mm_set1_ps(2.0); // 2 in yn+1 = 2*|x*y| + yn
    __m128 v_x = _mm_setzero_ps(); // All x values are set to 0 cuz z0=0
    __m128 v_y = _mm_setzero_ps(); // All y values are set to 0 cuz z0=0
    __m128i v_iteration_count = _mm_setzero_si128(); // Vector of iteration counts of the corresponding pixels
    __m128i v_checkVariable = _mm_set1_epi32(1); // Vector of check variable (explanation: see below)
    __m128i v_whileCondition; // Vector of if conditions of the corresponding pixels in the loop
    __m128i v_ifTerminated = _mm_set1_epi32(1); // Vector of ifTerminated value initial value is 1 (more on that see below)

    while (isAllZero(v_ifTerminated) == 0) {
        // Derivated functions of our parameter x and y in our complex burning fractal function
        __m128 xSquared = _mm_mul_ps(v_x, v_x); // x values all squared
        __m128 ySquared = _mm_mul_ps(v_y, v_y); // y values all squared (this does not change)
        __m128 xyMul = _mm_mul_ps(v_x, v_y); // x * y
        __m128 xyMulAbs = vectorialAbs(xyMul); // |x*y|
        __m128 v_xx = _mm_add_ps(_mm_sub_ps(xSquared, ySquared), pixels_x); // Calculating the new x value
        __m128 v_yy = _mm_add_ps(_mm_mul_ps(xyMulAbs, eqConstant), pixels_y); // Calculating the new y value
        v_x = v_xx; // Recursion of the x value
        v_y = v_yy; // Recursion of the y value
        __m128 xySquaredSum = _mm_add_ps(xSquared,ySquared);

        //this is the while condition ((x * x + y * y) < LIMIT) && (iteration_count < n) for the 4 different pixels of 4 x values
        //_mm_castps_si128(_mm_cmplt_ps(xySquaredSum, LIMIT)) corresponds to (x * x + y * y) < LIMIT)
        //_mm_cmplt_ps(xySquaredSum, LIMIT) if the condition is true, fills all the 32 bits of the segments with 0xffffffff = -1 otherwise 0
        //_mm_castps_si128(...) casts the float vector to an integer vector.
        //_mm_cmpeq_epi32(v_iteration_count, v_n) corresponds to (iteration_count == n)
        //_mm_andnot_si128(...) for the and (negation of the first element, then and)

        v_whileCondition = _mm_andnot_si128(_mm_cmpeq_epi32(v_iteration_count, v_n), _mm_castps_si128(_mm_cmplt_ps(xySquaredSum, LIMIT)));
        
        /*
        This variable makes all the 0xffffffff=-1 a 0x00000001=1 by anding those two so the condition can only be true=1 or false=0
        This variable checks which of the x values left the loop concerning our while condition  
        It is also our loop variable, so if the loop is for an x value of the corresponding pixel terminated,
        we just simply add our ifTerminated variable which is 0. Otherwise we add 1 which means it still loops
        */
        v_ifTerminated = _mm_and_si128(v_whileCondition, v_checkVariable);

        // Add the ifTerminated value for the ones which are still looping
        v_iteration_count = _mm_add_epi32(v_iteration_count, v_ifTerminated); //iteration_count++;

    }
    return v_iteration_count; // Returning the iteration count which is the required parameter to calculate the right colors
}

void painter_p(__m128i iteration_to_escape, unsigned char *img, unsigned img_index, unsigned n) //assigns the correct rgb values to our img array
{
    int32_t *element = (int32_t *) &iteration_to_escape;
    int greenConstant = 31;
    
    for (size_t i = 0; i < 4; i++)
    {
        if (element[i] == n) { // If a coordinate converges, paint it black directly
        img[img_index] = 0;                 // Blue
        img[img_index+1] = 0;               // Green
        img[img_index+2] = 0;               // Red   
    } else { // If a coordinate diverges, calculate its corresponding colors
        img[img_index] = 0;                             // Blue
        img[img_index+1] = element[i] * greenConstant;       // Green
        img[img_index+2] = 255;                         // Red
    } 
        img_index += 3; 
    }
}

void burning_ship_p(float complex start, size_t width, size_t height, float res, unsigned n, unsigned char *img)
{
    float start_x = creal(start);
    float start_y = cimag(start);
    float temp_x = start_x;
    float temp_y = start_y;
    
    unsigned img_index = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width-(width%4); j+=4) {
            float pixel_x_array[] = {temp_x, temp_x+res,temp_x+(res*2),temp_x+(res*3)}; // Array of the 4 x values of 4 pixels
            float pixel_y_array[] = {temp_y,temp_y,temp_y,temp_y}; // Array of the 4 y values of 4 pixels
            __m128i vector_n = _mm_set1_epi32(n);// Creates a vector of 4 n values
            __m128 vector_x = _mm_loadu_ps(pixel_x_array); // It gets 4 x values of 4 pixels and makes it a 128 bit vector
            __m128 vector_y = _mm_loadu_ps(pixel_y_array); // It gets 4 x values of 4 pixels and makes it a 128 bit vector           
            __m128i iteration_to_escape = iterate_pixel_p(vector_x, vector_y, vector_n); // Calculate the iteration count
            painter_p(iteration_to_escape, img, img_index, n);
            img_index += 12;
            temp_x += (res*4);
        }
        for (size_t a = 0; a < (width%4); a++) // If width%4 != 0, then we process the residual pixels manually (non-parallel)
        {
            unsigned iteration_to_escape = iterate_pixel(temp_x, temp_y, n); // Calculate the iteration count
            painter(iteration_to_escape, img, img_index, n);
            img_index += 3;
            temp_x += res;
        }
        temp_x = start_x; // Reset the x value
        temp_y -= res;// Reset the y value 
                      // It should be y-res, otherwise the ship would be upside down
    }
}
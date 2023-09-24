#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define _LIMIT 4 //our convergence/divergence threshold

unsigned iterate_pixel(float pixel_x, float pixel_y, unsigned n) // Check if the coordinates are either converge and diverge
{
    float x = 0.0;
    float y = 0.0;
    unsigned iteration_count = 0;
    while (((x * x + y * y) < _LIMIT) && (iteration_count < n)) {
        // Derivated functions of our parameter x and y in our complex burning fractal function
        float x_n = x * x - y * y + pixel_x;
        float y_n = 2 * fabs(x * y) + pixel_y;
        x = x_n;
        y = y_n;
        iteration_count++;
    }
    return iteration_count; // Returning the iteration count which is the required parameter to calculate the right colors
}

void painter(unsigned iteration_to_escape, unsigned char *img, unsigned img_index, unsigned n) // Assigns the correct rgb values to our img array
{
    int greenConstant = 31;
    if (iteration_to_escape == n) { // If a coordinate converges, paint it to black directly
        img[img_index] = 0;                 // Blue
        img[img_index+1] = 0;               // Green
        img[img_index+2] = 0;               // Red   
    } else { // If a coordinate diverges, calculate its corresponding colors
        img[img_index] = 0;                                    // Blue
        img[img_index+1] = iteration_to_escape * greenConstant;     // Green //the greater the greenConstant, 
                                                                    //the yellower become the pixels next to the black pixels 
        img[img_index+2] = 255;                                // Red
    }  
}



void burning_ship(float complex start, size_t width, size_t height, float res, unsigned n, unsigned char *img)
{
    float start_x = creal(start);
    float start_y = cimag(start);
    float temp_x = start_x;
    float temp_y = start_y;
    
    unsigned img_index = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) {
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
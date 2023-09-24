#include "bmpp.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 2


typedef struct arguments
{
    FILE* imageFile;
    bmpImage_p* image; 
    size_t sizeOfARow;
    size_t startLine; //unique for each thread 
    size_t endLine; //unique for each thread
    unsigned char* startptr;
    size_t pos;
}passedArguments;


void bmpImageHeaderInit_p(bmpHeader_p* header, int width, int height){
    header->size = (3 * width + (width%4))* height; //each row should be a multiple of 4 bytes, 
                                                                        //otherwise we need padding concerning the size of the row, so +(width%4)
	header->reserved1 = 0;
    header->reserved2 = 0;
	header->offset = 54; //54 bytes of total bmp header file
	header->DIB_header_size = 40; 
	header->image_width = width;
	header->image_height = height;
	header->planes = 1;
	header->bits_per_pixel = 24; //24 Bit BMP file
	header->compression = 0;
	header->image_size = 0;
	header->xPixels_pm = 0;
	header->yPixels_pm = 0;
	header->colors = 0;
	header->important_colors = 0; 
}


// Sets the pixels of bmpImage image to img and fills the header.
void bmpImageSet_p(bmpImage_p* image, int width, int height, unsigned char* img){
	image->pixels = img;
    bmpImageHeaderInit_p(&image->header, width, height);
}

//make each thread writes its own line
void* bmpImageWriteLines_p(void* args){  
    passedArguments* arguments = (passedArguments*) args;
    unsigned char padding[3] = {'\0', '\0', '\0'}; //maximum number of padding must be 3 due to modulo
    fseek(arguments->imageFile,  arguments->pos, SEEK_SET);
    for (size_t y = 0; y < (arguments->image->header.image_height/NUM_THREADS) + 100; y++) //writing all the pixels to the file
	{
        fwrite(arguments->startptr, sizeof(unsigned char), arguments->sizeOfARow, arguments->imageFile); //writing a row of pixels
	    fwrite(padding, sizeof(unsigned char), (arguments->image->header.image_width) % 4, arguments->imageFile); //writing the padding to the file
        arguments->startptr += arguments->sizeOfARow;   
    }   
    fclose(arguments->imageFile);
 
    return NULL;
}

// Writes the bmpImage image to a file called fileName.
void bmpImageWrite_p(bmpImage_p* image, char* fileName){
    passedArguments args[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    FILE* imageFile = fopen(fileName, "wb"); //since it's a bmp file write it in binary
    if (imageFile == NULL)
	{
		fprintf(stderr, "File cannot be created.\n");
		return;
	}
    
    size_t numberOfRows = image->header.image_height;
    unsigned short bmpSignature = 0x4D42; //"BM" in Ascii but in Little-Endian
    fwrite(&bmpSignature, sizeof(bmpSignature), 1, imageFile); //to write in binary we use fwrite
    fwrite(&image->header, sizeof(bmpHeader_p), 1, imageFile); //writing the bmpHeader to our image file
	for (size_t i = 0; i < NUM_THREADS; i++)
    {
        args[i].image = image;
        args[i].imageFile = fopen(fileName, "wb");
        args[i].sizeOfARow = 3*image->header.image_width;
        args[i].startLine = i * (numberOfRows / NUM_THREADS);
        args[i].endLine = (i+1) * (numberOfRows / NUM_THREADS);
        args[i].startptr = args[i].image->pixels + (i * args->sizeOfARow * (numberOfRows / NUM_THREADS));
        
        size_t numberOfLines = args[i].endLine - args[i].startLine;
        size_t paddingLength = (args[i].image->header.image_width) % 4;
        size_t lineOffset = i * (args[i].sizeOfARow + paddingLength) * numberOfLines;
        size_t pos = lineOffset + sizeof(unsigned short) + sizeof(bmpHeader_p);
        args[i].pos = pos;

        if (pthread_create(&threads[i], NULL, bmpImageWriteLines_p, &args[i]))
        {
            fprintf(stderr, "Rendering was terminated due to an error.\n");
        }
    }
    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(threads[i], NULL))
        {
            fprintf(stderr, "Rendering was terminated due to an error.\n");
        }
    }
	fclose(imageFile);
}




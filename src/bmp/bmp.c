#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>

void bmpImageHeaderInit(bmpHeader* header, int width, int height){
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
void bmpImageSet(bmpImage* image, int width, int height, unsigned char* img){
	image->pixels = img;
    bmpImageHeaderInit(&image->header, width, height);
}

// Writes the bmpImage image to a file called fileName.
void bmpImageWrite(bmpImage* image, char* fileName){
    FILE* imageFile = fopen(fileName, "wb"); //since it's a bmp file write it in binary
	if (imageFile == NULL)
	{
		fprintf(stderr, "File cannot be created.\n");
		return;
	}
	
    unsigned short bmpSignature = 0x4D42; //"BM" in Ascii but in Little-Endian
    fwrite(&bmpSignature, sizeof(bmpSignature), 1, imageFile); //to write in binary we use fwrite
    fwrite(&image->header, sizeof(bmpHeader), 1, imageFile); //writing the bmpHeader to our image file
    
    size_t numberOfRows = image->header.image_height;
	size_t sizeOfARow = 3*image->header.image_width;
	unsigned char padding[3] = {'\0', '\0', '\0'}; //maximum number of padding must be 3 due to modulo

	for (size_t y = 0; y < numberOfRows; y++) //writing all the pixels to the file
	{
		fwrite(image->pixels, sizeof(unsigned char), sizeOfARow, imageFile); //writing a row of pixels
		fwrite(padding, sizeof(unsigned char), ((image->header.image_width)%4), imageFile); //writing the padding to the file
		image->pixels += sizeOfARow;
	} 
	fclose(imageFile);
}


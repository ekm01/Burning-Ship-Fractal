#include <stdio.h>
#include <stdint.h>

typedef struct bmpHeader //BMP file format from wikipedia: "https://en.wikipedia.org/wiki/BMP_file_format" //bmpHeader has the size of 54 bytes
{
  uint32_t size; //size of the file  
  uint16_t reserved1; //not used in our implementation       
  uint16_t reserved2; //not used in our implementation     
  uint32_t offset; //offset to our pixelArray
  uint32_t DIB_header_size; //DIB header size //DIB header has the size of 40 bytes
  int32_t image_width;         
  int32_t image_height;        
  uint16_t planes; //number of color planes  
  uint16_t bits_per_pixel; 
  uint32_t compression; //in our implementation the file is uncompressed, so it's equal to 0     
  uint32_t image_size; //size of our bmpSize
  int32_t xPixels_pm; 
  int32_t yPixels_pm; 
  uint32_t colors; //number of colors  
  uint32_t important_colors; 
}bmpHeader;

typedef struct bmpImage //our bmp image has a bmp header and a pixel array
{
    struct bmpHeader header;
    unsigned char* pixels; //it is our img array that we have in our burningship() as a parameter
                          //it has the rgb colors of the corresponding pixels so 3 (char) elements per pixel
}bmpImage;

void bmpImageHeaderInit(bmpHeader* header, int width, int height);

void bmpImageSet(bmpImage* image, int width, int height, unsigned char* img);

void bmpImageWrite(bmpImage* image, char* fileName);
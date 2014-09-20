/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512

typedef uint8_t BYTE;

int main(int argc, char* argv[])
{
    // TODO
    FILE* file = fopen("card.raw", "r");
    FILE* img;
    BYTE data[512];
    char title[8];
    int in_image = 0;
    int img_counter = 0;
    
    while (fread(data, BLOCK_SIZE, 1, file) == 1)
    {
        if (data[0] == 0xff && data[1] == 0xd8 && data[2] == 0xff && (data[3] == 0xe0 || data[3] == 0xe1))
        {
            sprintf(title, "%03d.jpg", img_counter);
            
            if (in_image == 1)
            {
                fclose(img);
            }
            
            img = fopen(title, "w");
            img_counter++;
            in_image = 1;
        }
        
        if (in_image == 1)
        {
            fwrite(data, BLOCK_SIZE, 1, img);
        }
    }
    
    fclose(img);
    
    return 0;
}

/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy n infile outfile\n");
        return 1;
    }

    // remember resize multiplier
    int resize_num = atoi(argv[1]);
    
    if (resize_num < 1 || resize_num > 100)
    {
        printf("Parameter n must be between 1 and 100\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // change header info
    int infile_biWidth = bi.biWidth;
    int infile_biHeight = abs(bi.biHeight);
    
    bi.biWidth *= resize_num;
    bi.biHeight *= resize_num;
    int old_image_size = bi.biSizeImage;
    bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) * abs(bi.biHeight);
    int image_size_diff = bi.biSizeImage - old_image_size;
    bf.bfSize += image_size_diff;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int infile_padding =  (4 - (infile_biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outfile_padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // set initial scanline position
    int new_scanline_pos = ftell(inptr);
    
    // iterate over infile's scanlines
    for (int i = 0; i < infile_biHeight; i++)
    {
        // repeat infile's scanline to resize vertically
        for (int resize_counter = 0; resize_counter < resize_num; resize_counter++)
        {
            // reseting position to the start of the scanline
            fseek(inptr, new_scanline_pos, SEEK_SET);
            
            // iterate over pixels in scanline
            for (int j = 0; j < infile_biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // repeat infile's current pixel to resize horizontally
                for (int k = 0; k < resize_num; k++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // then add it back (to demonstrate how)
            for (int k = 0; k < outfile_padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
        
        // reseting to the start of the next line
        new_scanline_pos += (infile_biWidth * 3) + infile_padding;
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}

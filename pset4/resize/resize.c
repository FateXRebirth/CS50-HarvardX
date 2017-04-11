#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

// chose proper position to write
int selectPos(int pos, float factor);
// check color of pixel
void checkColor(RGBTRIPLE pixel);

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        // ensure proper usage
        fprintf(stderr,"Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    float n = atof(argv[1]);
    
    if( n < 0.0 || n > 100.0)
    {
        // ensure proper usage
        fprintf(stderr,"n must be a positive integer less than or equal to 100.\n");
        return 1;
    }
    char *infile = argv[2];
    char *outfile = argv[3];
    
    FILE *inptr = fopen(infile,"r");
    if(inptr==NULL)
    {
        fprintf(stderr,"The input file can't open for reading.\n");
        return 1;
    }
    
    FILE *outptr = fopen(outfile,"w");
    if(outptr==NULL)
    {
        fprintf(stderr,"The output file can't open for writing.\n");
        return 1;
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
        return 1;
    }
    
    // variable for storing data from input file
    BITMAPFILEHEADER outbf = bf;
    BITMAPINFOHEADER outbi = bi;
    
    // determine resized bmp size
    outbi.biHeight = bi.biHeight * n;
    outbi.biWidth = bi.biWidth * n;
    
    // ensure proper pixel
    if( n < 1.0 && (bi.biWidth%2 != 0) )
    {
        outbi.biHeight -= 1;
        outbi.biWidth += 1;
    }
    
    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outpadding =  (4 - (outbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // determine resized bmp size
    outbi.biSizeImage = ((sizeof(RGBTRIPLE) * outbi.biWidth) + outpadding ) * abs(outbi.biHeight);
    outbf.bfSize = outbi.biSizeImage + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
   
    // write outfile's BITMAPFILEHEADER
    fwrite(&outbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&outbi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // preprocessing
    // factor is proportion of original bmp size and resized bmp size
    float factor = (float)outbi.biWidth / (float)bi.biWidth;
    
    // temporary storage
    RGBTRIPLE * scanline = calloc(bi.biWidth, sizeof(RGBTRIPLE));
    
    // store temporary data
    RGBTRIPLE triple;
    
    // position of output cursor
    int rowPos = 0;
    
    // variable to check if jump
    int jump;
    
    // iterate over infile's scanlines
    for(int i = 0 , biHeight = abs(bi.biHeight) ; i < biHeight && rowPos < outbi.biWidth ;)
    {
        // determine if jump
        jump = selectPos(rowPos,factor);
        // if in the same row
        if(jump == i)
        {
            // read all pixel in original bmp
            for(int j = 0 ; j < bi.biWidth ; j++)
            {
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                scanline[j] = triple;
            }
            // write specific pixel to resized bmp
            for(int k = 0 ; k < outbi.biWidth ; k++)
            {
                triple = scanline[selectPos(k,factor)];
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            // padding
            for(int l = 0 ; l < outpadding ; l++)
            {
                fputc(0x00, outptr);
            }
            rowPos++;
            i++;
            // move to the next row
            fseek(inptr, padding, SEEK_CUR);
        }
        // need to jump
        else if(jump > i)
        {
            // how far to cross
            int distance = abs(jump-i);
            for(int m = 0 ; m < distance ; m++)
            {
                fseek(inptr, sizeof(RGBTRIPLE)*bi.biWidth, SEEK_CUR);
                fseek(inptr, padding, SEEK_CUR);
            }
            i = i+distance;
            // read all pixel in original bmp
            for(int r = 0 ; r < bi.biWidth ; r++)
            {
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                scanline[r] = triple;
            }
            // write specific pixel to resized bmp
            for(int o = 0 ; o < outbi.biWidth ; o++)
            {
                triple = scanline[selectPos(o,factor)];
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            // padding
            for(int p = 0 ; p < outpadding ; p++)
            {
                fputc(0x00, outptr);
            }
            rowPos++;
            i++;
            // move to the next row
            fseek(inptr, padding, SEEK_CUR);
        }
        // need to back
        else
        {
            // write again
            // write specific pixel to resized bmp
            for(int q = 0 ; q < outbi.biWidth ; q++)
            {
                triple = scanline[selectPos(q,factor)];
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            // padding
            for(int s = 0 ; s < outpadding ; s++)
            {
                fputc(0x00, outptr);
            }
            rowPos++;
        }
    }
    
    // in case of last time losing
    while(rowPos < outbi.biWidth)
    {
        for(int q = 0 ; q < outbi.biWidth ; q++)
            {
                triple = scanline[selectPos(q,factor)];
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            // padding
            for(int s = 0 ; s < outpadding ; s++)
            {
                fputc(0x00, outptr);
            }
            rowPos++;
    }
    
    // release memory...
    free(scanline);
   
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

// calculate right position
int selectPos(int pos, float factor)
{
    int temp = (float)pos;
    // if factor > 1.0 , down
    if( factor > 1.0)
    {
        return (int)floor(temp/factor);
    }
    // otherwise, up
    else
    {
        return (int)ceil(temp/factor);
    }
}

void checkColor(RGBTRIPLE pixel)
{
    if ( pixel.rgbtRed == 0xff && pixel.rgbtGreen == 0xff && pixel.rgbtBlue == 0xff)
        fprintf(stderr,"white...%i %i %i\n", pixel.rgbtRed, pixel.rgbtGreen, pixel.rgbtBlue);
    else if ( pixel.rgbtGreen == 0xff)
    {
        fprintf(stderr,"Green...%i %i %i\n", pixel.rgbtRed, pixel.rgbtGreen, pixel.rgbtBlue);
    }
    else
    {
        fprintf(stderr,"Unknown...%i %i %i\n", pixel.rgbtRed, pixel.rgbtGreen, pixel.rgbtBlue);
    }
}
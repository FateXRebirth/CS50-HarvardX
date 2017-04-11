#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        // ensure proper usage
        fprintf(stderr,"Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    int n = atoi(argv[1]);
    
    if( n < 0 || n > 100)
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
    
    // variable for storing data from input file
    BITMAPFILEHEADER bf2;
    BITMAPINFOHEADER bi2;
    BITMAPINFOHEADER bi1 = bi;
    
    // change the size
    bi2.biHeight = bi.biHeight * n;
    bi2.biWidth = bi.biWidth * n ;
    
    // determine padding for scanlines
    int padding =  (4 - (bi1.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding2 =  (4 - (bi2.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    bi2.biSizeImage = ((sizeof(RGBTRIPLE)*bi2.biWidth)+padding2)*abs(bi2.biHeight);
    bf2.bfSize = bi2.biSizeImage+sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 1;
    }
    
    bf.bfSize = bf2.bfSize;
    bi.biSizeImage = bi2.biSizeImage;
    bi.biHeight = bi2.biHeight;
    bi.biWidth = bi2.biWidth;
   
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // Variable for indexing temporary array
    int index = 0 ;
    
    // iterate over infile's scanlines
    for(int i = 0 , biHeight = abs(bi1.biHeight) ; i < biHeight ; i++)
    {
        RGBTRIPLE *tri = (RGBTRIPLE*)malloc(sizeof(RGBTRIPLE)*bi2.biWidth);
        for(int j = 0 ; j < bi1.biWidth ; j++)
        {
            RGBTRIPLE triple;
            
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            // repeat n times pixel horizontally
            for(int k = 0 ; k < n ; k++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                // store value in temporary array 
                tri[index] = triple;
                index++;
            }
        }
        if(index == bi2.biWidth)
        {
            // Reset index for next time
            index = 0 ;
        }
        
        // Padding
        for(int p = 0 ; p < padding2 ; p++)
        {
            fputc(0x00, outptr);
        }
        
        // repeat n times pixel vertically
        for(int a = 0 ; a < n-1 ; a++)
        {
            for(int b = 0 ; b < bi2.biWidth ; b++)
            {
                // Get value from temporary array and write it
                RGBTRIPLE temp = tri[b];
                fwrite(&temp, sizeof(RGBTRIPLE), 1, outptr);
            }
            // repeat n times pixel vertically
            for(int c = 0 ; c < padding2 ; c++)
            {
                fputc(0x00, outptr);
            }
        }
        // Release the memory in case it crashs.
        free(tri);
        // skip the padding
        fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
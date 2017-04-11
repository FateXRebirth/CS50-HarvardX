#include <stdio.h>
#include <stdbool.h>


int main(int argc, char *argv[])
{   
    // ensure proper usage
    if ( argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // get input file name
    char * infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    // ensure it exist
    if(inptr == NULL)
    {
        fprintf(stderr, "This file can't be opend for reading.\n");
        return 1;
    }
    
    // buffur for storing each block data
    unsigned char buffer[512];
    // name for each img
    int name = 0;
    // char array storing name
    char filename[8];
    // check if file is opening
    bool open = false;
    FILE *img = NULL;
    
    // keep reading until reach end of file
    while(fread(&buffer, 1, sizeof(buffer), inptr) == 512)
    {
        // if we get the header
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if(!open)
            {
                sprintf(filename, "%03i.jpg", name);
                name++;
                img = fopen(filename, "w");
                open = true;
                if(img == NULL)
                {
                    fprintf(stderr, "This file already exist , so it can't be opend for writing.\n");
                    return 1;
                }
                fwrite(&buffer, 1, sizeof(buffer), img);
            }
            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", name);
                name++;
                img = fopen(filename, "w");
                open = true;
                if(img == NULL)
                {
                    fprintf(stderr, "This file already exist , so it can't be opend for writing.\n");
                    return 1;
                }
                fwrite(&buffer, 1, sizeof(buffer), img);
            }
        }
        //if we don't get header
        else
        {
            if(open)
            {
                fwrite(&buffer, 1, sizeof(buffer), img);
            }
        }
        
    }
    
    fclose(inptr);
    
    // success
    return 0;
}
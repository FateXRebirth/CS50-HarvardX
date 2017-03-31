#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int n = 0;
    // ensure proper usage
    do
    {
        printf("Height: ");
        n = get_int();
    }while(n < 0 || n > 23);
    
    int x = 2;
    int y = n-1;
    
    // print #
    for ( int i = 0 ; i < n ; i ++)
    {
        for ( int j = y; j > 0 ; j --)
        {
            printf(" ");
        }
        for ( int k = 0 ; k < x ; k ++)
        {
            printf("#");
        }
        x++;
        y--;
        printf("\n");
    }
    
    return 0;
}
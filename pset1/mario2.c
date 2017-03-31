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
    
    int x = n-1;
    int y = 1;
    int z = 2;
    
    // print #
    for ( int i = 0 ; i < n ; i ++)
    {
        for ( int j = x; j > 0 ; j --)
        {
            printf(" ");
        }
        for ( int k = 0 ; k < y ; k ++)
        {
            printf("#");
        }
        for ( int w = 0 ; w < z ; w ++)
        {
            printf(" ");
        }
        for ( int v = 0 ; v < y ; v ++)
        {
            printf("#");
        }
        
        x--;
        y++;
        
        printf("\n");
    }
    
    return 0;
}
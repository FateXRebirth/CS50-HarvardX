#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Minutes: ");
    // get number
    int n = get_int();
    
    // multiple 12
    printf("Bottles: %i\n", n*12);
    
    return 0;
}
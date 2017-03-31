#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    printf("O hai! ");
    
    float x;
    
    // ensure proper usage
    do
    {
        printf("How much change is owed?\n");
        x = get_float();
    }while(x<0);

    // deal with number convert
    int n = (int) (x * 100 + 0.5);
    
    // variable for each type of coins
    int count = 0;
    int quarter = 25;
    int dime = 10;
    int nickel = 5;
    int penny = 1;
    
    // count how many coins need
    while ( n >= quarter)
    {
        count++;
        n = n - quarter;
    }
    
    while ( n >= dime)
    {
        count++;
        n = n - dime;
    }  
    
    while ( n >= nickel)
    {
        count++;
        n = n - nickel;
    }
    
    while ( n >= penny)
    {
        count++;
        n = n - penny;
    }
    
    printf("%i\n",count);
    
    return 0;
}
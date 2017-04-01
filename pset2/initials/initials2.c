#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string s = get_string();
    
    // capitalize the beginner
    if(s[0]!=' ')
    {
        printf("%c",toupper(s[0]));
    }
    // capitalize
    for(int i = 0 , n = strlen(s) ; i < n-1 ; i++)
    {
         if(s[i]==' ' &&  s[i+1]!= ' ')
         {
            printf("%c", toupper(s[i+1]));
         }
    }
    printf("\n");
    return 0;
}
    
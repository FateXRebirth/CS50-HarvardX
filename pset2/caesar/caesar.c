#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // ensure proper usage
    if(argc < 2 || argc > 2)
    {
        printf("Usage: ./caesar k\n");
        exit(1);
    }else if(argv[1]<0)
    {
        printf("k must be a non-negative integer.\n");
        exit(1);
    }
    else
    {
        int k = atoi(argv[1]);
        // variable for alphabetical 
        int index;
        // variable for saving
        int temp;
        printf("plaintext: ");
        string s = get_string();
        for(int i = 0 , n = strlen(s) ; i < n ; i ++)
        {
            if(isalpha(s[i]))
            {
                //A letter's part
                if(isupper(s[i]))
                {
                    index = s[i]-65;
                    temp = ( index + k ) % 26;
                    s[i] = (char)(temp+65);
                }
                //a letter's part
                else
                {
                    index = s[i]-97;
                    temp = ( index + k ) % 26;
                    s[i] = (char)(temp+97);
                }
            }
        }
        printf("ciphertext: %s\n", s);
        return 0;
    }
}
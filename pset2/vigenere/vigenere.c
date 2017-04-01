#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// select key from a string
int selectK(string k , int index)
{
    if(isupper(k[index]))
    {
        return k[index]-65;
    }
    else
    {
        return k[index]-97;
    }
}

int main(int argc, string argv[])
{
    // ensure proper usage
    if(argc < 2 || argc > 2)
    {
        printf("Usage: ./vigenere k\n");
        exit(1);
    }
    else
    {
        string keyword = argv[1];
        // check if all strings are alphabetical
        for(int i = 0 , n = strlen(keyword) ; i < n ; i ++)
        {
            if(!isalpha(keyword[i]))
            {
                printf("Illegal input\n");
                exit(1);
            }
        }
        // variable for length of keyword;
        int lengthK = strlen(keyword);
        // variable for k
        int k;
        // variable for chosing K
        int indexK = 0;
        // variable for alphabetical 
        int index;
        // variable for saving
        int temp;
        printf("plaintext: ");
        string s = get_string();
        for(int i = 0 , n = strlen(s) ; i < n ; i ++)
        {
            if(indexK==lengthK)
            indexK = 0;
            if(isalpha(s[i]))
            {
                //A letter's part
                if(isupper(s[i]))
                {
                    index = s[i]-65;
                    k = selectK(keyword,indexK);
                    temp = ( index + k ) % 26;
                    s[i] = (char)(temp+65);
                }
                //a letter's part
                else
                {
                    index = s[i]-97;
                    k = selectK(keyword,indexK);
                    temp = ( index + k ) % 26;
                    s[i] = (char)(temp+97);
                }
            indexK++;
            }
        }
        printf("ciphertext: %s\n", s);
        return 0;
    }
}
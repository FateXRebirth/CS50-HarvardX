#define _XOPEN_SOURCE
#include <unistd.h>
#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>

// trying all possibile password
bool tryPassword(char* plain, char* cipher, char* salt)
{   
    char dict[] = {"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    int dictLength = strlen(dict);
    
    // variable for representing each digit
    int first = 0;
    int second = 1;
    int third = 2;
    int forth = 3;
    
    // ensure a proper end of each string
    dict[4] = '\0';
    salt[2] = '\0';
    cipher[4] = '\0';
    
    // trying password XXXX
    for(int i = 0 ; i < dictLength ; i++ )
    {
        cipher[first]=dict[i];
        for(int j = 0 ; j < dictLength ; j++)
        {
            cipher[second]=dict[j];
            for(int k = 0 ; k < dictLength ; k++)
            {
                cipher[third]=dict[k];
                for(int l = 0 ; l < dictLength ; l++)
                {
                    cipher[forth]=dict[l];
                    if( ! strcmp ( crypt(cipher, salt), plain ) ) 
                    {
                        return true;
                    }
                }
                
            }
        }
    }
    
    // trying password XXX
    cipher[forth]='\0';
    for(int i = 0 ; i < dictLength ; i++ )
    {
        cipher[first]=dict[i];
        for(int j = 0 ; j < dictLength ; j++)
        {
            cipher[second]=dict[j];
            for(int k = 0 ; k < dictLength ; k++)
            {
                cipher[third]=dict[k];
                if( ! strcmp ( crypt(cipher, salt), plain ) ) 
                {
                    return true;
                }
            }
        }
    }
    // trying password XX
    cipher[third]='\0';
    for(int i = 0 ; i < dictLength ; i++ )
    {
        cipher[first]=dict[i];
        for(int j = 0 ; j < dictLength ; j++)
        {
            cipher[second]=dict[j];
            if( ! strcmp ( crypt(cipher, salt), plain ) ) 
            {
                return true;
            }   
        }
    }
    
    // trying password X
    cipher[second]='\0';
    for(int i = 0 ; i < dictLength ; i++)
    {
        cipher[first]=dict[i];
        if( ! strcmp ( crypt(cipher, salt), plain ) ) 
        {
            return true;
        }   
    }
    
    return false;
}

int main(int argc, string argv[])
{
    // ensure proper usage
    if ( argc < 2 || argc > 2)
    {
        printf("Illegal input\n");
        exit(1);
    }
    
    char salt[2];
    
    // get salt from input
    strncpy(salt, argv[1], 2);
    
    int plainLength = strlen(argv[1]);
    char plaintext[plainLength];
    // get plain text from input
    strcpy(plaintext,argv[1]);
    char ciphertext[4];
    
    // trying
    if(tryPassword(plaintext,ciphertext,salt))
    {
        // success
        //printf("Matched\n");
        printf("%s\n",ciphertext);
        return 0;
    }
    
    // failure
    //printf("Not Match\n");
    
    return 1;
}
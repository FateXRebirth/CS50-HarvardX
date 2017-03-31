#include <stdio.h>
#include <cs50.h>
#include <math.h>

// get coefficient of each bit
long long getCoefficient(int n)
{
    long long x = pow(10,n);
    return x;
}

int main(void)
{
    printf("Number: ");
    long long n = get_long_long();
    
    //variable for calculating
    long long sum = 0;
    long long sum1 = 0;
    long long sum2 = 0;
    long long temp1 = 0;
    long long temp2 = 0;
    long long temp3 = 0;
    int first;
    int second;
    
    for(int i = 0 ; i < 16 ; i++)
    {
        // sort even bit out
        if(i%2 == 0)
        {
            temp1 = (n/getCoefficient(i))%getCoefficient(1);
            second = temp1;
            sum1 = sum1+temp1;
        } 
        // sort odd bit out
        else
        {
            temp2 = (n/getCoefficient(i))%getCoefficient(1);
            first = temp2;
            temp2 = temp2*2;
            if(temp2>=10)
            {
                temp3 = temp2%10;
                temp2 = temp2/10;
                temp2 = temp2 + temp3;
            }
            sum2 = sum2 + temp2;
        }
    }
   
    sum = sum1+sum2;
    
    // determine type of each card
    if(sum%10 != 0)
    {
        printf("INVALID\n");
    }
    else
    {
        if(first == 5)
        {
            printf("MASTERCARD\n");
        }
        else if(first == 4)
        {
            printf("VISA\n");
        }
        else if(second == 3 && first == 0)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("VISA\n");
        }
    }
    
    return 0;
}
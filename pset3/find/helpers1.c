/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"
#include "string.h"

/**
 * Returns true if value is in array of n values, else false.
 */

// binary search 
bool search(int value, int values[], int n)
{
    // implement an O(log n) searching algorithm
    if(n<0) return false;
    int middle = n/2;
    int left = 0;
    int right = n;
    while(right-left>0)
    {
        middle = (right+left)/2+1;
        // if the first value is target
        if(value == values[left])
        {
            return true;
        }
        // if the middle value is target
        else if(value == values[middle])
        {
            return true;
        }
        // if the target is smaller than the middle one , then search left half
        else if(value < values[middle])
        {
            right = middle-1;
        }
        // if the target is larger than the middle one , then search right half
        else
        {
            left = middle+1;
        }
    }
    // if target is not in container
    return false;
}

/**
 * Sorts array of n values.
 */

void sort(int values[], int n)
{
    // implement an O(n^2) sorting algorithm
    for(int i = 0 ; i < n ; i++)
    {
        for(int j = i+1 ; j < n ; j++)
        {
            if(values[i] > values[j])
            {
                int temp = values[j];
                values[j] = values[i];
                values[i] = temp;
            }
        }
    }
    return;
}

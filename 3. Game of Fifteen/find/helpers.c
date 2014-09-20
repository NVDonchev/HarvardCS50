/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // Binary search
    int min = 0;
    int mid_point = n / 2;
    int max = n - 1;
    
    while (min <= max)
    {
        if (value < values[mid_point])
        {
            max = mid_point - 1;
        }
        else if (value > values[mid_point])
        {
            min = mid_point + 1;
        }
        else
        {
            return true;
        }
        
        mid_point = (min + max) / 2;
    }
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // Bubble sort 
    bool sorted = false;
    while (!sorted)
    {
        sorted = true;
        for (int i = 0; i < n - 1; i++)
        {
            if (values[i] > values[i + 1])
            {
                int temp_holder = values[i];
                values[i] = values[i + 1];
                values[i + 1] = temp_holder;
                
                sorted = false;
            }
        }
    }
    
    return;
}

#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    int quarter = 25;
    int dime = 10;
    int nickel = 5;
    int penny = 1;
    int coins_counter = 0;
    float input = 0;
    
    printf("O hai! ");
    
    do
    {
        printf("How much change is owed?\n");
        input = GetFloat();
    } while (input < 0);
    
    int change = (int)round(input * 100);
    
    while (change > 0)
    {
        if (change >= quarter)
        {
            change -= quarter;
        }
        else if (change >= dime)
        {
            change -= dime;
        }
        else if (change >= nickel)
        {
            change -= nickel;
        }
        else
        {
            change -= penny;
        }
        
        coins_counter++;
    }
    
    printf("%d\n", coins_counter);
}

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height = 0;
    
    do
    {
        printf("Height: ");
        height = GetInt();
    } while (height < 0 || height > 23);
    
    int blank_count = height - 1;
    
    for (int rows = 0; rows < height; rows++)
    {
        for (int cols = 0; cols <= height; cols++)
        {
            if (cols < blank_count)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        
        blank_count--;
        printf("\n");
    }
}

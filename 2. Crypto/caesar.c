#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define ALPHABET_LENGTH 26

int CypherChar(char ch, int key, char alphabet_starting_num);

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        int key = atoi(argv[1]);
        string text = GetString();
        
        for (int i = 0, n = strlen(text); i < n; i++)
        {
            if (islower(text[i]))
            {
                text[i] = CypherChar(text[i], key, 'a');
            }
            else if (isupper(text[i]))
            {
                text[i] = CypherChar(text[i], key, 'A');
            }
        }
        
        printf("%s\n", text);
    }
    else
    {
        printf("Usage %s <key>\n", argv[0]);
        return 1;
    }
}

int CypherChar(char ch, int key, char alphabet_starting_num)
{
    return (((ch + key) - alphabet_starting_num) % ALPHABET_LENGTH) + alphabet_starting_num;
}

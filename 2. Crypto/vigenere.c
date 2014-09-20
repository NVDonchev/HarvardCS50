#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define ALPHABET_LENGTH 26

int CypherChar(char ch, int key, char alphabet_starting_num);

int main(int argc, string argv[])
{
    int keyword_length;
    bool key_legal = false;
    
    // Checking if the keyword parameter is present
    if (argc == 2)
    {
        keyword_length = strlen(argv[1]);
        
        // Checking if the keyword contains only letters
        int letter_count = 0;
        for (int i = 0; i < keyword_length; i++)
        {
            if (isalpha(argv[1][i]))
            {
                letter_count++;
            }   
        }
        if (letter_count == keyword_length)
        {
            key_legal = true;
        }
    }
    
    // Checking if everything is ok with the keyword
    if (key_legal)
    {
        string keyword = argv[1];
        
        // Setting all keyword letters to lower case
        for (int i = 0; i < keyword_length; i++)
        {
            if (isupper(argv[1][i]))
            {
                keyword[i] = tolower(keyword[i]);
            }
        }
        
        // Getting the text for encrypting from the user
        string text = GetString();
        
        // Encrypting the text
        bool keyword_letter_used = true;
        int keyword_index = 0;
        for (int i = 0, n = strlen(text); i < n; i++)
        {
            // Setting the keyword index
            if (keyword_letter_used && i > 0)
            {
                if (keyword_index < keyword_length - 1)
                {
                    keyword_index++;
                }
                else if (keyword_index == keyword_length - 1)
                {
                    keyword_index = 0;
                }
            }
            
            // Setting the key to apply
            int key = keyword[keyword_index] - 'a';
            
            // Applying the key to the text symbol by symbol
            if (isalpha(text[i]))
            {
                if (islower(text[i]))
                {
                    text[i] = CypherChar(text[i], key, 'a');
                }
                else if (isupper(text[i]))
                {
                    text[i] = CypherChar(text[i], key, 'A');
                }
                
                keyword_letter_used = true;
            }
            else
            {
                keyword_letter_used = false;
            }
        }
        
        // Printing the encrypted text
        printf("%s\n", text);
    }
    else
    {
        // Printing error message and returning error code 1
        printf("Usage %s <key>\n", argv[0]);
        return 1;
    }
}

int CypherChar(char ch, int key, char alphabet_starting_num)
{
    return (((ch + key) - alphabet_starting_num) % ALPHABET_LENGTH) + alphabet_starting_num;
}

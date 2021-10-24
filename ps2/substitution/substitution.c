#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

const int N = 26; // Number of letter in alphabet
const int AL = 97; // Decimal correpondent to a in ASCII
const int ZL = 122; // Decimal correpondent to z in ASCII
const int AU = 65; // Decimal correpondent to A in ASCII
const int ZU = 90; // Decimal correpondent to Z in ASCII

bool verifyKey(string key);
char sortChar(char letter);
void generateCiphertext(string plainText, string key);

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Error\n");
        return 1;
    }

    string key = argv[1];
    printf("%s\n", key);

    bool isRightKey = verifyKey(key);

    if (!(isRightKey))
    {
        printf("Enter a valid key!\n");
        return 1;
    }

    string plainText = get_string("plainText: ");

    generateCiphertext(plainText, key);

    return 0;
}

// Verify if the key is correct
bool verifyKey(string key)
{
    int len = strlen(key);
    char alphaFull [N];

    if (len != N){ // Must have exactly 26 characters
        return false;
    }

    for(int i = 0; i < len; i++)
    {
        if (sortChar(key[i]) != 'n') // Check if all chars are alphabetical, if not return false
        {
            alphaFull[(int) tolower(key[i]) - AL] = key[i];
        }
        else
        {
            return false;
        }
    }
    for (int i = 0; i < N; i++)
    {
        if(sortChar(alphaFull[i]) == 'n') // Check if there is an repeted letter
        {
            return false;
        }
    }
    return true;
}

// Sorts chars into alphabetical uppercase, alphabetical lowercase and non-alfabetical
char sortChar(char letter)
{
    if (((int) letter >= AL) && ((int) letter <= ZL))
    {
        return 'l'; // lowercase char
    }
    else if (((int) letter >= AU) && ((int) letter <= ZU))
    {
        return 'u'; // uppercase char
    }
    else
    {
        return 'n'; // non Aplhabetical
    }
}

// Generates a ciphertext according to a key and an input
void generateCiphertext(string plainText, string key)
{
    int len = strlen(plainText);
    char ciphertext [len]; // Array of ciphertext chars

    printf("ciphertext: ");
    for(int i = 0 ; i < len; i++)
    {
        if(sortChar(plainText[i]) == 'l')
        {
            ciphertext[i] = tolower(key[(int) plainText[i] - AL]); // Insert ciphered lowercase char into the right position of the array
            printf("%c", ciphertext[i]);
        }
        else if(sortChar(plainText[i]) == 'u')
        {
            ciphertext[i] = toupper(key[(int) plainText[i] - AU]); // Insert ciphered uppercase char into the right position of the array
            printf("%c", ciphertext[i]);
        }
        else
        {
            ciphertext[i] = plainText[i]; // Insert non Alphabetical char into the right position of the array
            printf("%c", ciphertext[i]);
        }
    }
    printf("\n");
}

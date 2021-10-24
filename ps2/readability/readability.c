#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>


// Defining prototypes
int countLetters(string text);
bool isAlpha(char letter);
int countWords(string text);
int countSentences(string text);
void colemanLiau(int totalLetters, int totalWords, int totalSentences);
bool isPunc(char punc);

// Main function
int main(void)
{
    // Ask user for input
    string text = get_string("Enter a text: ");
    if ((countLetters(text) == 0) || (countSentences(text) == 0)) // Check if the input given is valid
    {
        printf("Enter a valid text.\n");
        return -1;
    }
    colemanLiau(countLetters(text), countWords(text), countSentences(text));
}

// This function return the grade of the text according to the Coleman-Liau formula
void colemanLiau(int totalLetters, int totalWords, int totalSentences)
{
    float l = 100 * (float) totalLetters / totalWords;
    float s = 100 * (float) totalSentences / totalWords;
    int index = round(0.0588 * l - 0.296 * s - 15.8);

    if (index < 0)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) index);
    }
}

// This function counts the number of letters in the text
int countLetters(string text)
{
    int count = 0;

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isAlpha(text[i]))
        {
            count++;
        }
    }
    return count;
}

// This function returns the number of words in a text
int countWords(string text)
{
    int count = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if ((isAlpha(text[i])) && (isPunc(text[i + 1])))
        {
            count++;
            continue;
        }
        else if ((isAlpha(text[i])) && (text[i + 1] == ' '))
        {
            for (int j = i; j < len; j++)
            {
                if (isAlpha(text[j]))
                {
                    count++;
                    i = j;
                    break;
                }
            }
        }
    }
    return count;
}

// This function returns the number of sentences in a text
int countSentences(string text)
{
    int count = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if ((text[i] == '.') || (text[i] == '!') || (text[i] == '?')) // End of a sentence
        {
            count++;
        }
    }
    return count; // Return the number of senteces
}


// This function returns true if the character is Alphabetical and false on the contrary
bool isAlpha(char letter)
{
    if ((((int) letter > 64) && ((int) letter < 91)) || (((int) letter > 96)
            && ((int) letter < 123))) // Checking in ASCII table if is a char uppercase or lowercase
    {
        return true;
    }
    else
    {
        return false;
    }
}

// This function returns false if there is a valid
// character that can be put between letters of a word.
// It will return true otherwise.
bool isPunc(char punc)
{
    if ((!(isAlpha(punc))) && (punc != '-') && (punc != '\''))
    {
        return true;
    }
    else
    {
        return false;
    }
}
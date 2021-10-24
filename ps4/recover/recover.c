#include <stdio.h>
#include <stdlib.h>


int findJPEG(unsigned char *block);
void createJPEG(unsigned char *block, char filename[3], int sizeOfBlock);

int isJPEG;
int JPEGNumber = 0;
char fileName[3];
int countFile = 0;

int main(int argc, char *argv[])
{
    // Opens the file
    FILE *card = fopen(argv[1], "r");

    // Incorrect input/file not found
    if (card == NULL)
    {
        return 1;
    }

    // Initializes variables
    int sizeOfBlock = 512;
    unsigned char *block;
    int *aux;
    int end;

    // Searches on the entire file to looking for an image
    while (1)
    {
        block = malloc(sizeOfBlock);
        end = fread(block, sizeOfBlock, 1, card);
        isJPEG = findJPEG(block);

        // Hited the end of the file
        if (end < 1 && JPEGNumber > 0)
        {
            free(block);
            fclose(card);
            return 0;
        }

        // Appends data to an existing file
        else if (isJPEG == 0 && JPEGNumber > 0)
        {
            createJPEG(block, fileName, sizeOfBlock);
        }

        // Creates a new file and appends the first block of data to it
        else if (isJPEG == 1)
        {
            sprintf(fileName, "%03i.jpg", JPEGNumber);
            createJPEG(block, fileName, sizeOfBlock);
            JPEGNumber ++;
        }

        // File not found in this block of data
        else
        {
            free(block);
            continue;
        }
        //Avoids memory leaking
        free(block);
    }
}

// Checks if the blocks
int findJPEG(unsigned char *block)
{
    if (*block == 0xff && *(block + 1) == 0xd8 && *(block + 2) == 0xff && (*(block + 3) & 0xf0) == 0xe0)
    {
        return 1;
    }
    return 0;
}

// Appends data to a JPEG image
void createJPEG(unsigned char *block, char filename[3], int sizeOfBlock)
{
    int isjpeg = 0;
    FILE *file;
    file = fopen(filename, "a");
    fwrite(block, sizeOfBlock, 1, file);
    fclose(file);
}
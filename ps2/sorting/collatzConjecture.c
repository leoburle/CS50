#include <stdio.h>
#include <cs50.h>

int collatzAlg(int i, int count);

int main(void)
{
    int input = get_int("Enter an integer: ");
    int i = collatzAlg(input, 0);
}

int collatzAlg (int i, int count)
{
    if(i == 1)
    {
        printf("Arrived in 1 and it took %i\n", count);
        return count;
    }
    else if (i % 2 == 0)
    {
        count ++;
        collatzAlg(i/2, count);
        return count;
    }
    else
    {
        count ++;
        collatzAlg(3*i+1, count);
        return count;
    }
}
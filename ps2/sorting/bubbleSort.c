#include <stdio.h>
#include <cs50.h>


int ARRAY[10] = {5, 143, 12, 1, 54, 123, 23, 9, 4, -2};
int ARRAY2[10] = {5, 143, 12, 1, 54, 123, 23, 9, 4, -2};
const int LEN = 10;

void bubbleSort(void);
void bubbleSortRecursive(int size);

int main(void)
{
    bubbleSort();
    bubbleSortRecursive(LEN);
    for(int i = 0; i < LEN; i++){
        printf("%i ", ARRAY2[i]);
    }
    printf("\n");
}

void bubbleSort(void){
    for (int i = 0; i < LEN; i++){
        for (int j = 0; j < LEN - i-1; j++){
            int aux = ARRAY[j];
            if (ARRAY[j] > ARRAY[j+1]){
                ARRAY[j] = ARRAY[j+1];
                ARRAY[j+1] = aux;
            }
        }
        printf("%i\n", ARRAY[LEN-1]);
    }

    for (int i = 0; i < LEN; i++){
        printf ("%i ", ARRAY[i]);
    }
    printf("\n");
}

void bubbleSortRecursive(size){
    if(size == 1){
        return;
    }

    for (int i = 0; i < size-1; i++){
        bubbleSortRecursive(size-1);
        if(ARRAY2[i] > ARRAY2[i+1]){
            int aux = ARRAY2[i];
            ARRAY2[i] = ARRAY2[i+1];
            ARRAY2[i+1] = aux;
            //printf("%i ", i);
        }
    }
}
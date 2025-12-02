// Intro_to _programming_w2_excercise 1.1
#include <stdio.h>

int main(){
    int a, b;
    printf("Please enter 2 integers, split by space:");
    scanf("%d %d", &a, &b);
    // implicit conversion: (a and b are integers, divided by 2 prompts to double)
    double average = (a + b) / 2.0;

    printf("Average = %.2f\n", average);
    return 0;

}
// Intro_to _programming_w2_excercise 2.2
#include <stdio.h>

int main(){
    char ch;
    int num;
    printf("Enter a number");
    scanf("%c", &ch);

    printf("Enter a number");
    scanf("%d", &num);

    //implicit conversion: char is promoted to int
    int sum = ch + num;
    // Explicit conversion: force int back into char
    char sumChar = (char)sum;

    printf("Sum as integer: %d/n", sum);
    printf("sum as character (ASCII) %c\n", sumChar)
    return 0;
}
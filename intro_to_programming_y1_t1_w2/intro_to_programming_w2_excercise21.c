// Intro_to _programming_w2_excercise 2.1 (a)
#include <stdio.h>

int P1(){
    char ch;
    printf("Enter a character");
    scanf(" %c", &ch); //the space before %c ignores any new line
    
    //explicit type casting
    int ascii = (int)ch;
    printf("Character: %c, ASCII value is %d\n", ch, ascii);
    return 0;
}



// Intro_to _programming_w2_excercise 2.1 (b)
#include <stdio.h>

int P2(){
    int ascii;
    printf("Enter an ASCII value (1-255)");
    scanf("%d", &ascii);

    //explicit type casting
    char ch = (char)ascii;

    printf("ASCII value: %d, Character : %c\n", ascii, ch);
    return 0;
}
int main () {
    P1();
    P2();
    return 0;
}
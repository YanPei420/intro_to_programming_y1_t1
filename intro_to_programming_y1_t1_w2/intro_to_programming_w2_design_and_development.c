// intro_to_programming_w2_design_and_development

#include <stdio.h>
//example 1: adding two numbers
int example1(){

    //initialise variables
    double num1, num2, sum;

    //get input
    printf("Enter your first number:");
    scanf("%lf", &num1);
    printf("Enter your second number:");
    scanf("%lf", &num2);

    //add and output
    sum = num1 +num2;
    printf("The sum of %.2f, and %.2lf is %.2lf",num1, num2, sum);
    return 0;
}
#include <stdio.h>
int leap_year_example(){
    int year;
     printf("please enter a year");
     scanf("%d", &year);
     if (year%4 == 0 && year%100 != 0) || (year%400 == 0){
        printf("This is a leap year")
     }else{
        printf("this is not a leap year")
     }
     return 0;
}

int excersise_about_switch_statements(){
    int grade_number;
    printf("Please enter your grade");
    scanf("%d", grade_number);

    switch(grade_number){
        case (grade_number>= 90 && grade_number <= 100):
            printf("%d corresponds to a grade A", grade_number);
            break;
        case (grade_number>= 80 && grade_number <= 89):
            printf("B");
            break;
        case (grade_number>= 70 && grade_number <= 79):
            printf("C");
            break;
        case (grade_number>= 60 && grade_number <= 69):
            printf("D");
            break;
        default:
            printf("F")
    }
}

int loops_practice_example(){
    int num;
    printf("please enter a number between 1-100");
    scanf("%d", &num);
    
}

int main(){
    leap_year_example();
    excersise_about_switch_statements();
    return 0;
}
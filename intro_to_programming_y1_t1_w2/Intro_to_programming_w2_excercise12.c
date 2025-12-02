// Intro_to _programming_w2_excercise 1.2
#include <stdio.h>
#include <math.h>

int main(){
    double celcius, fahrenheit;
    printf("Enter temperature in celcius");
    scanf("%1f", &celcius);
    fahrenheit = (celcius *9.0/5.0) + 32.0;

    // Explicit conversion with rounding
    int roundedF = (int)round(fahrenheit);
    printf("Temperature in fahrenheit (rounded) is %d\n", roundedF);
    return 0;
}


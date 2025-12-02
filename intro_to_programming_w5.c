#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function declarations
void convertToWords(int n, char *words);
void trimSpaces(char *str);

const char *units[] = {
    "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",//first one is 0 so nothing
    "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen",
    "sixteen", "seventeen", "eighteen", "nineteen"
};

const char *tens[] = {
    "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy",//the above
    "eighty", "ninety"
};

// Function to convert number into words (British English style)
void convertToWords(int n, char *words) {
    if (n == 0) {
        strcat(words, "zero");
        return;
    }

    if (n == 1000000) {
        strcat(words, "one million");
        return;
    }

    if (n >= 1000) {
        int thousands = n / 1000;
        convertToWords(thousands, words);
        strcat(words, " thousand");
        n %= 1000;
        if (n > 0) strcat(words, " ");
    }

    if (n >= 100) {
        int hundreds = n / 100;
        strcat(words, units[hundreds]);
        strcat(words, " hundred");
        n %= 100;
        if (n > 0) strcat(words, " and ");
    }

    if (n > 0) {
        if (n < 20) {
            strcat(words, units[n]);
        } else {
            int tens_digit = n / 10;
            int ones_digit = n % 10;
            strcat(words, tens[tens_digit]);
            if (ones_digit) {
                strcat(words, " ");
                strcat(words, units[ones_digit]);
            }
        }
    }
}

// Function to remove extra spaces
void trimSpaces(char *str) {
    int i = 0, j = 0;
    int len = strlen(str);
    int spaceFlag = 0;

    // Skip leading spaces
    while (i < len && isspace(str[i])) i++;

    // Copy while removing extra spaces
    for (; i < len; i++) {
        if (isspace(str[i])) {
            if (!spaceFlag) str[j++] = ' ';
            spaceFlag = 1;
        } else {
            str[j++] = str[i];
            spaceFlag = 0;
        }
    }

    // Remove trailing spaces
    if (j > 0 && isspace(str[j - 1])) j--;
    str[j] = '\0';
}

int main() {
    int number;
    char words[500] = "";

    printf("Enter an integer between 1 and 1000000: ");
    scanf("%d", &number);

    // Validate range
    if (number < 1 || number > 1000000) {
        printf("Number out of range!\n");
        return 1;
    }

    // Convert number
    convertToWords(number, words);

    // Clean up spacing
    trimSpaces(words);

    printf("In words: %s\n", words);
    return 0;
}

#include <stdio.h>
/*array sum calculation
int main (){
    int array[10];
    int i, n, sum = 0;
    printf("input no.elements you want to put in array");
    scanf("%d", &n);
    for (i=0; i<n; i++){
        printf("enter the %dth for the array", i+1);
        scanf("%d", &array[i]);
        sum += array[i];
        printf("%d", sum);
    }
    return 0;
}
*/

/*finding duplicates in an array
int main (){
    int array[10];
    int i, j, n, count = 0;
    printf("input number of elements you'd like to enter");
    scanf("%d", &n);
    for (i = 0; i < n; i++){
        printf("enter a number");
        scanf("%d", &array[i]);
    }
    for (i = 0; i < n; i++){
        for (j = i + 1; j < n; j++){
            if (array[i] == array[j]){
                count++;
            }
        }
    }
    printf("there are %d duplicates", count);
    return 0;
}
*/

/*print unique elements
int main(){
    int i, j, n, count = 0;
    int array[10];
    printf("how many elements would you like to enter?");
    scanf("%d", &n);
    for (i = 0; i < n; i++){
        printf("please enter a number");
        scanf("%d", &array[i]);
    }
    for (i = 0; i < n; i++){
        count = 0;
        for (j = 0; j < n + 1; j++){
            if (i != j){
                if (array[i] == array [j]){
                    count++;
                }
            }
        }
        if (count == 0){
            printf("unique elements: %d \n", array[i]);
        }
    }
    return 0;
}
*/

/*merge two arrays of the same size sorted in descending order
int main(){
    int array1[100], array2[100], array3[200];
    int n, i, j, c = 0;
    printf("how many elelments would you like to enter?");
    scanf("%d", &n);
    for (i = 0; i < n; i++){
        printf("enter the elements for the first array");
        scanf("%d", &array1[i]);
    }
        for (i = 0; i < n; i++){
        printf("enter the elements for the second array");
        scanf("%d", &array2[i]);
    }
    for (i = 0; i < n; i++){
        array3[i] = array1[i];
    }
    i = n;
    for (j = 0; j < n; j++){
        array3[i] = array2[j];
        i++;
    }
    printf("the combined array is \n");
    for (i = 0; i < 2*n; i++){
        printf("%d", array3[i]);
    }

    for (i = 0; i < (2*n); i++){
        for (j = 0; j < (2*n)-1; j++){
            if (array3[j] <= array3[j+1]){
                c = array3[j+1];
                array3[j+1] = array3[j];
                array3[j] = c;
            }
        }
    }
    for (i = 0; i < 2*n; i++){
        printf("\nthe combined array in decreasing order is: %d", array3[i]);
    }
    return 0;
}
*/

/*finding max and min and average
int main(){
    int array[10];
    int n, i, max, min, avg;
    printf("enter the number of elements you'd like to enter in the array");
    scanf("%d", &n);
    for (i = 0; i < n; i++){
        printf("enter the elements for your array");
        scanf("%d", &array[i]);
    }
    max = array[0];
    min = array[0];
    for (i = 0; i < n; i++){
        if (array[i] < min){
            min = array[i];
        }
        if (array[i] > max){
            max = array[i];
        }
    }
    printf("the array is:");
    for (i = 0; i < n; i++){
        printf("%d", array[i]);
    }
    avg = (max + min)/2;
    printf("\nthe maximum is: %d \n", max);
    printf("the minimum is: %d \n", min);
    printf("the average is: %d", avg);
    return 0;
}
*/

/* inserting an array, will do it in ascending order
int main (){
    int array[10];
    int i, j, n, a, b, c;
    printf("how many elements do you want in the array?");
    scanf("%d", &n);
    for (i = 0; i < n; i++){
        printf("enter the element %d for the array", i+1);
        scanf("%d", &array[i]);
    }
    printf("the array you've entered is:\n");
    for (i = 0; i < n; i++){
        printf("%d", array[i]);
    }
    for (i = 0; i < n; i++){
        for (j = i+1; j < n; j++){
            if (array[i] >= array[j]){
                b = array[i];
                array[i] = array[j];
                array[j] = b;
            }
        }
    }
    printf("\nthe sorted (ascending) array is: \n");
    for (i = 0; i < n; i++){
        printf("%d", array[i]);
    }
    printf("\nenter a value that you'd like to enter: ");
    scanf("%d", &a);
    for (i = 0; i < n; i++){
        if (a < array[i]){
            c = i;
            break;
        }else{
            c = i + 1;
        }
    }
    for (i = n; i >= c; i--){
        array[i+1] = array[i];
    }
    array[c] = a;
    printf("the new array is:");
    for (i = 0; i < n+1; i++){
        printf("%d", array[i]);
    }
    return 0;
}
*/

/*delete element at desired position of an array
int main(){
    int array[10];
    int i, n, num, a;
    printf("enter the number of elements you want for this array");
    scanf("%d", &n);
    for (i = 0; i < n; i++){
        printf("enter the element %d for the array", i+1);
        scanf("%d", &array[i]);
    }
    printf("the array is:\n");
    for (i = 0; i < n; i++){
        printf("%d, ", array[i]);
    }
    printf("\nwhich element, starting from the first, would you like to delete?\n");
    scanf("%d", &num);
    a = num -1;
    while (a < n){
        array[a] = array[num];
        a++;
    }
    n--;
    printf("the new array is: \n");
    for (i = 0; i < n; i++){
        printf("%d", array[i]);
    }
    return 0;
}
*/


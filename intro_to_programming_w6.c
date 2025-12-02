#include <stdio.h>

//exercise1: Print the frequency (of each element) and unique elements in array
int main(){
    int arr[10];
    int n, i, j, count;

    printf("how many elements would you like to insert?");
    scanf("%d", &n);

    for (i = 0; i < n; i++){
        printf("enter element %d: ", i+1);
        scanf("%d", &arr[i]);
    }

    printf("the array you've entered is:\n");
    for (i = 0; i < n; i++){
        printf("%d", arr[i]);
    }

    for (i = 0; i < n; i++){
        for (j = i+1; j <= n; j++){
            count = 1;
            if (arr[i] == arr[j]){
                count++;
            }
        }
        printf("\nThe element: %d, occurs %d times\n", arr[i], count);
    }

    return 0;
}
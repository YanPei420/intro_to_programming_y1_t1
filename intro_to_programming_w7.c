#include <stdio.h>

/*exercise1
int main()
{ char ch = 'Z';
char *cptr = &ch;

int a = 1;
int *num = &a;

printf("cp is %p\n", cptr);
printf("The character at cptr is %c\n", *cptr);
// Pointer arithmetic - check what cptr + 1 is
cptr = cptr + 1;
printf("cptr is %p\n", cptr);
// Do not print *cptr, because it points to
// memory space not allocated to your program

//for numbers:
printf("num is %p\n", num);
printf("number at num is: %d\n", *num);

num = num + 1;
printf("num+1 is: %p\n", num);
return 0;
}
*/

/*exercise2
int main()
{
    int arr[10];
    int *ptr;
    int i, j;

    for (i = 0; i < 10; i++)
    {
        printf("\nplease enter the %d element of the array : ", i+1);
        scanf("%d",&arr[i]);
    }

    printf("for reverse order: ");
    for (j = 9; j >= 0; j--)
    {
        printf("%d, ", arr[j]);
    }
    printf("\n");
return 0;
}
*/

/* exercise3 以放弃

int main()
{
    // make 3x3 array and take inputs
    int i, j, k;
    int arr[3][3];

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            // instead of: printf("\nplease enter the number for the %d column and %d row: ", i+1, j+1);, leave this and type using spaces
            scanf("%d", &arr[i][j]);
        }
    }

    printf("the array you've entered is: \n");
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    //for clockwise
    printf("for clockwise: \n");
    for (j = 0; j < 3 ; j++)
    {
        for (i = 0; i < 3 ; i++)
        {
            printf("%d", arr[i][j]);
        }
        printf("\n");
    }

    return 0;
}
*/

/*exercise 4
#include <stdio.h>

int A[2][4][4];
int i, j, k, n, a, b, c;

int n = 10;

//getting values for which element you want using pass by refernece
void element_of_array(int *a, int *b,int *c){
    printf("the element of the array you've entered is: A[%u][%u][%u] = %d\n", *a, *b, *c, *(*(*(A + *a)+ *b)+ *c));
}

int main(){

    //initilising the array
    for (i = 0; i < 2; i++){
        for (j = 0; j < 4; j++){
            for (k = 0; k < 4; k++){
                *(*(*(A+i)+j)+k) = n;
                n++;
            }
        }
    }

     //printing the array
    for (i = 0; i < 2; i++){
        for (j = 0; j < 4; j++){
            for (k = 0; k < 4; k++){
                printf("A[%d][%d][%d] = %d, \n", i, j, k, *(*(*(A+i)+j)+k));
            }
        }
    }

    //printing the accessed array
    printf("the array is of the form [2][4][4], please enter which element you'd like to find number by number ([a][b][c])");
    printf("\n please enter a: ");
    scanf("%d", &a);
    printf("\n please enter b: ");
    scanf("%d", &b);
    printf("\n please enter c: ");
    scanf("%d", &c);
    element_of_array(&a, &b, &c);
    
    return 0;
}
*\


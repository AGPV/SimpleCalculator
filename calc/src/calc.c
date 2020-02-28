/*
 ============================================================================
 Name        : calc.c
 Author      : Nikita Agapov
 Version     :
 Copyright   : (͡° ͜ʖ ͡°)
 Description : calculator
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
	float x = 0, y = 0, result = 0;
	char ch, res='R';                       //ch - symbol of operation
	while (res == 'R'){                     //for repeat(res as flag)
	printf("Enter first number ");
	scanf(" %f", &x);
	printf("\n");
	printf("Choose operation(+,-,*,/,!,^) ");
	scanf(" %c", &ch);
	printf("\n");
	if (ch == '!'){                         //to not ask for a second number for factorial
	    if ( (float)((int)x) == x){         //checking for an integer
	    result = 1;
	    if (x>0){
	        for (int i = 1; i<x+1; i++){
	            result *= i;
	        }
	    } else if (x<0) {                   //workaround for x<0
	        //if you don't need calculate factorial for negative number uncomment next code
	        /*printf("I can't calculate factorial for negative number. ");
	          printf("\n");
	          goto restart;*/
	        //and comment this
	        x = -x;
	        for (int i = 1; i<x+1; i++){
	            result *= i;
	        }
	        result = -result;
	    }
	    } else {
	        printf("I can't calculate factorial for not integer number. \n");
	        goto restart;
	    }
	} else {
	    printf("Enter second number ");
	    scanf(" %f", &y);
	    printf("\n");
	    if (ch == '+'){
	        result = x + y;
	    } else if (ch == '-'){
	        result = x - y;
	    } else if (ch == '*'){
	        result = x * y;
	    } else if (ch == '/'){
	        if (y!=0){
	        result = x / y;
	        } else {
	            printf("I can't divide by zero!\n");
	            goto restart;
	        }
	    } else if (ch == '^'){
	        if ((x==0)&&(y==0)){                            //defense from 0^0
	            printf("Indeterminate form 0^0\n");
	            goto restart;
	        } else {
	        result = x;
	        if (y>0){
	            for (int i = 1; i<y; i++){
	            result *= x;
	            }
	        } else if (y<0){                                //replacing x^y on 1/(x^-y) for y<0
	            y = -y;
                for (int i = 1; i<y; i++){
                result *= x;
                }
                result = 1 / result;
	        } else result=1;
	        }
	    } else {
	        printf ("Wrong operation! \n");
	        goto restart;
	    }
	}
	if ( (float)((int) result ) == result){      //discarding zeros after the decimal point if the number is an integer.
	    int intResult = result;
	    printf(" =");
	    printf(" %i", intResult);
	    printf("\n");
	} else {
	    printf(" =");
	    printf(" %f", result);
	    printf("\n");
	}
	restart:
	printf("For restart enter R, for exit enter something different: ");
	scanf(" %c", &res);
	printf("\n");
	}
	return EXIT_SUCCESS;
}

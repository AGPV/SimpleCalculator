/*
 ============================================================================
 Name        : calc.c
 Author      : Nikita Agapov
 Version     : 1.01
 Copyright   : (͡° ͜ʖ ͡°)
 Description : Calculator. Enable operations +, -, *, /, !, ^
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    FILE *inFile, *outFile;
    int err = 0;                            //flag for wrong operations
    int vlen = 0;                           //vectors length
    float x = 0, y = 0, result = 0;
    float *v1, *v2;                         // vectors
    char inname[20], outname[20];
    char ch, res='R', vec;                  //ch - symbol of operation; vec - vector or number operation
    while (res == 'R'){                     //for repeat(res as flag)
    inFile = NULL;
    while(inFile == NULL){
    printf("Enter the name of the input file(less than 20 characters): ");
    gets(inname);
    inFile = fopen(inname, "r");
    if (inFile == NULL){
        printf("This file does not exist.\n");
    } else {
        printf("Enter the name of the output file(less than 20 characters): ");
        gets(outname);
        outFile = fopen(outname, "w");
    }
    }
    while(feof(inFile) == 0){               //work for end of file
    fscanf(inFile, " %c", &ch);             //operation
    fscanf(inFile, " %c", &vec);            //vector or number
    if (vec=='s'){
    fscanf(inFile, "%f", &x);               //first number
    if (ch == '!'){                         //to not ask for a second number for factorial
        if ( (float)((int)x) == x){         //checking for an integer
        result = 1;
        if (x>0){
            for (int i = 1; i<x+1; i++){
                result *= i;
            }
        } else if (x<0) {                   //workaround for x<0
            x = -x;
            for (int i = 1; i<x+1; i++){
                result *= i;
            }
            result = -result;
        }
        } else {
            fprintf(outFile, "I can't calculate factorial for not integer number.\n");
            err = 1;
        }
    } else {
        fscanf(inFile, "%f", &y);          //second number
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
                fprintf(outFile, "I can't divide by zero!\n");
                err = 1;
            }
        } else if (ch == '^'){
            if ((x==0)&&(y==0)){                            //defense from 0^0
                fprintf(outFile, "Indeterminate form 0^0.\n");
                err = 1;
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
            fprintf (outFile, "Wrong operation! \n");
            err = 1;
        }
    }
    if(err != 1){                                //if the error in the expression does not out the answer
    if((float)(int) x == x){
        fprintf(outFile, "%i %c ", (int)x, ch);
    } else {
        fprintf(outFile, "%f %c ", x, ch);
    }
    if((float)(int) y == y){
        fprintf(outFile, "%i = ", (int) y);
    } else {
        fprintf(outFile, "%f = ", y);
    }
    if ( (float)((int) result ) == result){      //discarding zeros after the decimal point if the number is an integer.
        fprintf(outFile, "%i\n", (int) result);
    } else {
        fprintf(outFile, "%f\n", result);
    }
    } else {
        err = 0;
    }
    if(feof(inFile) != 0){
        printf("For restart enter R, for exit enter something different: ");
        scanf(" %c", &res);
        printf("\n");
    }
    } else if (vec=='v'){
        fscanf(inFile, "%i", &vlen);
        v1 = malloc(vlen*sizeof(int));
        v2 = malloc(vlen*sizeof(int));
        fprintf(outFile, "(");
        for (int i = 0; i<vlen; i++){                   //scanning and printing first vector
            fscanf(inFile, " %f", &v1[i]);
            if ( (float)((int) v1[i]) == v1[i]){
                fprintf(outFile, "%i ", (int) v1[i]);
            } else {
                fprintf(outFile, "%f ", v1[i]);
            }
        }
        fprintf(outFile, ") %c ",ch);
        if (ch != 's'){                                 //to not scan second vector for scaling
            fprintf(outFile, "(");
            for (int i = 0; i<vlen; i++){               //scanning and printing second vector
                fscanf(inFile, " %f", &v2[i]);
                if ( (float)((int) v2[i]) == v2[i]){
                    fprintf(outFile, "%i ", (int) v2[i]);
                } else {
                    fprintf(outFile, "%f ", v2[i]);
                }
            }
            fprintf(outFile, ") = (");
            if (ch == '+'){
                for (int i = 0; i<vlen; i++){
                    if ( (float)((int) v1[i] + v2[i] ) == v1[i] + v2[i]){
                        fprintf(outFile, "%i ", (int) (v1[i] + v2[i]));
                    } else {
                        fprintf(outFile, "%f ", v1[i] + v2[i]);
                    }
                }
                fprintf(outFile, ")\n");
            } else if (ch == '-'){
                for (int i = 0; i<vlen; i++){
                    if ( (float)((int) v1[i] - v2[i] ) == v1[i] - v2[i]){
                        fprintf(outFile, "%i ", (int) (v1[i] - v2[i]));
                    } else {
                        fprintf(outFile, "%f ", v1[i] - v2[i]);
                    }
                }
                fprintf(outFile, ")\n");
            }
        } else if (ch == 's'){
            fscanf(inFile, " %f", &x);              //scalar
            fprintf(outFile, "%f = (", x);
            for (int i = 0; i<vlen; i++){
                if ((float)((int) v1[i] * x) == v1[i] * x){
                    fprintf(outFile, "%i ", (int) (v1[i] * x));
                } else {
                    fprintf(outFile, "%f ", v1[i] * x);
                }
            }
            fprintf(outFile, ")\n");
        } else fprintf(outFile, "Wrong operation!");
        free(v1);                           //freeing up memory
        free(v2);
        if(feof(inFile) != 0){
            printf("For restart enter R, for exit enter something different: ");
            scanf(" %c", &res);
            printf("\n");
        }
    }
    }
    fclose(inFile);
    fclose(outFile);
    }
    return EXIT_SUCCESS;
}

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

typedef struct in_list {
    char vec;               //vector or number operation
    char ch;                //symbol of operation
    int len;                //vectors length (if number len = 1)
    float *x;               //first number or vector
    float *y;               //second number or vector
    int err;                //flag for wrong operations
    struct in_list *next;
} in_data;

typedef struct res_list {
    float *result;
    struct res_list *res_next;
} out_data;

float *numbers(char sign, float *firstNum, float *secondNum);
float *vectors(char sign, int size, float *vector1, float *vector2);
float *read(FILE *input, int size);
void addElement(in_data *current, FILE *input);
void addElementInRes(out_data *current_res, in_data *current);

//calculation of operations with numbers
int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    FILE *inFile, *outFile;
    in_data *head, *current;
    out_data *headRes, *currentRes;
    char inname[20], outname[20];
    char restart='R';
    while (restart == 'R'){                     //for repeat(res as flag)
    inFile = NULL;
    while(inFile == NULL){
    printf("Enter the name of the input file(less than 20 characters): ");
    scanf("%s", inname);
    inFile = fopen(inname, "r");
    if (inFile == NULL){
        printf("This file does not exist.\n");
    } else {
        printf("Enter the name of the output file(less than 20 characters): ");
        scanf("%s", outname);
    }
    }
    head = malloc(1 * sizeof(in_data));
    fscanf(inFile, " %c %c", &head->ch, &head->vec);

    if (head->vec == 's'){
        head->len = 1;
    } else {
        fscanf(inFile, " %i", &head->len);
    }
    if (head->ch == '!') {
        head->x = read(inFile, head->len);
        head->y = NULL;
    } else {
        head->x = read(inFile, head->len);
        head->y = read(inFile, head->len);
    }

    current = head;
    while (feof(inFile)==0) //adding all elements in list
    {
        addElement(current, inFile);
        current = current->next;
    }
    fclose(inFile);
    headRes = malloc(sizeof(out_data));

    current = head;
    if (current->vec == 's') {
        headRes->result = numbers(current->ch, current->x, current->y);
    } else {
        headRes->result = vectors(current->ch, current->len, current->x, current->y);
    }
    headRes->res_next = NULL;
    current = current->next;
    currentRes = headRes;

    while (current != NULL) //adding all results to res_list
    {
        addElementInRes(currentRes, current);
        current = current->next;
        currentRes = currentRes->res_next;
    }
    current = head;
    currentRes = headRes;
    outFile = fopen(outname, "w");

    //writing data to a file
    while (current != NULL) {
        if (current->vec == 'v') {
            fprintf(outFile, "(");
            for (int i = 0; i < current->len; i++) {
                fprintf(outFile, " %.2f ", current->x[i]);
            }
            fprintf(outFile, ") %c (", current->ch);
            for (int i = 0; i < current->len; i++) {
                fprintf(outFile, " %.2f ", current->y[i]);
            }
            fprintf(outFile, ") = ");
            if (current->ch != '*') {
                fprintf(outFile, "(");
                for (int i = 0; i < current->len; i++) { fprintf(outFile, " %.2f ", currentRes->result[i]); }
                fprintf(outFile, ")\n");
            } else {
                fprintf(outFile, " %.2f\n", currentRes->result[0]);
            }
        } else if (current->vec == 's'){
            fprintf(outFile, "%.2f", current->x[0]);
            fprintf(outFile, " %c ", current->ch);
            fprintf(outFile, "%.2f",current->y[0]);
            fprintf(outFile, " = ");
            fprintf(outFile, "%f\n", currentRes->result[0]);
       }
        current = current->next;
        currentRes = currentRes->res_next;
    }
    fclose(outFile);
    printf("\nFor restart enter R, for exit enter something different: ");
    scanf(" %c", &restart);
    printf("\n");
    }
    return EXIT_SUCCESS;
}

//calculation of operations with numbers
float *numbers(char ch, float *x, float *y) {
    float *numResult;
    numResult = malloc(1 * sizeof(float));
    if (ch == '!'){                         //to not ask for a second number for factorial
            if ( (float)((int)x[0]) == x[0]){         //checking for an integer
            numResult[0] = 1;
            if (x>0){
                for (int i = 1; i<x[0]+1; i++){
                    numResult[0] *= i;
                }
            } else if (x<0) {                   //workaround for x<0
                x[0] = -x[0];
                for (int i = 1; i<x[0]+1; i++){
                    numResult[0] *= i;
                }
                numResult[0] = -numResult[0];
            }
            }
        } else {                             //second number
            if (ch == '+'){
                numResult[0] = x[0] + y[0];
            } else if (ch == '-'){
                numResult[0] = x[0] - y[0];
            } else if (ch == '*'){
                numResult[0] = x[0] * y[0];
            } else if (ch == '/'){
                if (y!=0){
                numResult[0] = x[0] / y[0];
                }
            } else if (ch == '^'){
                numResult = x;
                if (y>0){
                    for (int i = 1; i<y[0]; i++){
                    numResult[0] *= x[0];
                    }
                } else if (y<0){                                //replacing x^y on 1/(x^-y) for y<0
                    y[0] = -y[0];
                    for (int i = 1; i<y[0]; i++){
                    numResult[0] *= x[0];
                    }
                    numResult[0] = 1 / numResult[0];
                } else numResult[0]=1;
                }
            }
    return x;
    return y;
    return numResult;
    free(x);
    free(y);
    free(numResult);
}

//calculation of operations with vectors
float *vectors(char ch, int len, float *v1, float *v2) {
    float *vecResult;
        if (ch == '+'){
            vecResult = malloc(len * sizeof(float));
            for (int i = 0; i < len; i++)
            {
                vecResult[i] = v1[i] + v2[i];
            }
        } else if (ch == '-'){
            vecResult = malloc(len * sizeof(float));
            for (int i = 0; i < len; i++)
            {
                vecResult[i] = v1[i] - v2[i];
            }
        } else if (ch == 's'){
            vecResult = malloc(1 * sizeof(float));
            vecResult[0] = 0;
            for (int i = 0; i < len; i++)
            {
                vecResult[0] = vecResult[0] + (v1[i] * v2[i]);
            }
        }
    return vecResult;
    return v1;
    return v2;
    free(v1);
    free(v2);
    free(vecResult);
}

//read number or vector from file
float *read(FILE *input, int len){
    float *elem;
    elem = malloc(len * sizeof(float));
    for(int i = 0;i < len;i++)
    {
        fscanf(input,"%f", &elem[i]);
    }
    return elem;
}

//reading from file to list
void addElement(in_data *current, FILE *input){
    in_data *tmp = malloc(1 * sizeof(in_data));
    fscanf(input, " %c %c", &tmp->ch, &tmp->vec);
    if (tmp->vec == 'v'){
        fscanf(input, " %i", &tmp->len);}
    else{
        tmp->len = 1;
    }
    if (tmp->ch == '!')
    {
        tmp->x = read(input, tmp->len);
        tmp->y = NULL;
    }
    else
    {
        tmp->x = read(input, tmp->len);
        tmp->y = read(input, tmp->len);
    }
    tmp->next = NULL;
    current->next = tmp;
}


//add elements in result list
void addElementInRes(out_data *current_res, in_data *current)
{
    out_data *tmp_res = malloc(1 * sizeof(out_data));
    if (current->vec == 'n'){
        tmp_res->result = numbers(current->ch, current->x, current->y);
    }
    else{
        tmp_res->result = vectors(current->ch, current->len, current->x, current->y);
    }
    tmp_res->res_next = NULL;
    current_res->res_next = tmp_res;
}



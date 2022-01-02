#include <stdio.h> /*printf*/

#include "calculator.h"

int main()
{
    char *expression = "-1-5*  \t\v\f\n6+ (20-19)*2^2^3";
    double result = 0;
    Calculator(expression, &result);
    printf("result %f", result);
    
    return 0;
}

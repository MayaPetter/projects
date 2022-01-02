/******************************************
* Author:   Maya Petter                   *
* Reviewer:                               *
* Purpose:                                *
*******************************************/
#include <assert.h> /* assert  */
#include <stdlib.h> /* strtod  */
#include <ctype.h>  /* isdigit */
#include <math.h>   /* pow, floor */
#include <string.h> /* strlen  */

#include "calculator.h" /* calculator function */
#include "stack.h"      /* stack functions     */

#define ASCII_SIZE 256
#define STATES_NUM 2
#define NUM 0
#define OP 1
#define EXPO_PREC 3

#define UNUSED(x) ((void)(x)) /* for unused params in Error Invalid Expression*/

/*********** structs and typedef ***********************************************/
typedef enum state 
{
    WAIT_FOR_NUM, 
    WAIT_FOR_OP,
    ERROR,
    END
}state_t;

typedef struct calc_fsm
{
    state_t next_state;
    calc_errno_t (*action_func)(char **expression, stack_t *stack[]);
}calc_fsm_t;

typedef struct operator
{
    calc_errno_t (*operation_func)(double left, double right, stack_t *stack[]);
    int precedence;    
}operator_t;

typedef unsigned int u_int;

/*********** state functions **************************************************/
static calc_errno_t ErrorInvalidExp(char **expression, stack_t *stack[]);
static calc_errno_t PushNumToStack(char **expression, stack_t *stack[]);
static calc_errno_t PushOperatorToStack(char **expression, stack_t *stack[]);
static calc_errno_t FoldStack(char **expression, stack_t *stack[]);
static calc_errno_t PushBrackets(char **expression, stack_t *stack[]);
static calc_errno_t FoldBrackets(char **expression, stack_t *stack[]);
static calc_errno_t IgnoreWhiteSpace(char **expression, stack_t *stack[]);
static calc_errno_t CheckUnary(char **expression, stack_t *stack[]);

/*********** helper functions *************************************************/
static calc_errno_t Folding(stack_t *stack[]);
static int IsLowerPrecedence(char **expression, stack_t *stack[]);
static calc_errno_t LUTStackCreateInit(const char *expression);
static void LUTStackDestroy(stack_t *stack[]);

/*********** LUT init functions ***********************************************/
static void FSMInit();
static void OperatorLUTInit();

/*********** math functions ***************************************************/
calc_errno_t Addition(double left, double right, stack_t *stack[]);
calc_errno_t Subtraction(double left, double right, stack_t *stack[]);
calc_errno_t Multiply(double left, double right, stack_t *stack[]);
calc_errno_t Division(double left, double right, stack_t *stack[]);
calc_errno_t Exponentiation(double left, double right, stack_t *stack[]);

/*********** LUT declarations *************************************************/
static stack_t *LUT_STACK[2] = {NULL};
static calc_errno_t (*LUT_UNARY[2])() = {&ErrorInvalidExp, &PushNumToStack};
static calc_fsm_t CALC_FSM[ASCII_SIZE][STATES_NUM] = {0};
static operator_t LUT_OPERATOR[95] = {0};

/*********** Main function ****************************************************/
calc_errno_t Calculator(const char *expression, double *result)
{
    calc_errno_t status = CALC_SUCCESS;
    state_t state = WAIT_FOR_NUM;
    
    assert(NULL != expression);
    assert(NULL != result); 
    
    status = LUTStackCreateInit(expression);    
    
    /* check if FSM isn't initiate */
    if (WAIT_FOR_OP != CALC_FSM['0'][WAIT_FOR_NUM].next_state)
    {
        FSMInit();
        OperatorLUTInit();
    }
    
    while (END != state && ERROR != state && CALC_SUCCESS == status)
    {
        state_t next_state = CALC_FSM[(u_int)*expression][state].next_state;
        
        status = CALC_FSM[(u_int)*expression][state].action_func
                                              ((char **)&expression, LUT_STACK);
        state = next_state;
    }
    
    if (!StackIsEmpty(LUT_STACK[NUM]))
    {
        *result = *(double *)StackPeek(LUT_STACK[NUM]);
    }
    
    LUTStackDestroy(LUT_STACK);

    return status;
}

/*********** returns error when invalid expression ****************************/
static calc_errno_t ErrorInvalidExp(char **expression, stack_t *stack[])
{
    UNUSED(expression);
    UNUSED(stack);
    
    return CALC_ERR_INVALID_EXPRESSION;
}

/*********** Ignore white space in cases of tab, new line and space ***********/
static calc_errno_t IgnoreWhiteSpace(char **expression, stack_t *stack[])
{
    UNUSED(stack);
    assert(NULL != expression);

    ++(*expression);    
    
    return CALC_SUCCESS;
}

/*********** Checks if + or - is unary  ***************************************/
static calc_errno_t CheckUnary(char **expression, stack_t *stack[])
{
    assert(NULL != expression);
    assert(NULL != stack);

    return LUT_UNARY[0 != isdigit(*(*expression + 1))](expression, stack);
}

/*********** Push number to num_stack  ***************************************/ 
static calc_errno_t PushNumToStack(char **expression, stack_t *stack[])
{
    double num_to_push = 0.0;
    int push_status = STACK_OVERFLOW_ERROR;
    
    assert(NULL != expression);
    assert(NULL != stack);
    
    num_to_push = strtod(*expression, expression);
    push_status = StackPush(stack[NUM], &num_to_push);
    
    assert(STACK_OVERFLOW_ERROR != push_status);
        
    return CALC_SUCCESS;
}

/*********** Push operator to op_stack  ***************************************/
static calc_errno_t PushOperatorToStack(char **expression, stack_t *stack[])
{
    calc_errno_t status = CALC_SUCCESS;
    
    assert(NULL != expression);
    assert(NULL != stack);
    
    while (!StackIsEmpty(stack[OP]) && CALC_SUCCESS == status  
           && IsLowerPrecedence(expression, stack))
    {
        status = Folding(stack);
    }
    
    StackPush(stack[OP], *expression);

    ++(*expression);
    
    return status;
}

/*********** Fold function  ***************************************************/
static calc_errno_t Folding(stack_t *stack[])
{
    char operator = 0;
    double right_operand = 0.0;
    double left_operand = 0.0;
    calc_errno_t status = CALC_SUCCESS;
       
    assert(NULL != stack);
    
    right_operand = *(double *)StackPeek(stack[NUM]);
    StackPop(stack[NUM]);

    left_operand = *(double *)StackPeek(stack[NUM]);
    StackPop(stack[NUM]);

    operator = *(char *)StackPeek(stack[OP]);
    if (0 == LUT_OPERATOR[(u_int)operator].precedence)
    {
        StackPop(stack[OP]);
        return CALC_ERR_INVALID_EXPRESSION;
    }
    
    status = LUT_OPERATOR[(u_int)operator].operation_func
                                           (left_operand, right_operand, stack);
    StackPop(stack[OP]);
    
    return status;
}

/* Checks if expressions operator is in lower precedence then stack operator **/
static int IsLowerPrecedence(char **expression, stack_t *stack[])
{
    char operator = 0;
    int expression_precedence = 0;
    int stack_precedence = 0;
    
    assert(NULL != expression);
    assert(NULL != stack);
    
    expression_precedence = LUT_OPERATOR[(u_int)**expression].precedence;
    if (EXPO_PREC == expression_precedence)
    {
        return 0;
    } 
    
    operator = *(char *)StackPeek(stack[OP]);
    stack_precedence = LUT_OPERATOR[(u_int)operator].precedence;

    return expression_precedence <= stack_precedence;
}

/*********** Fold all stack ***************************************************/
static calc_errno_t FoldStack(char **expression, stack_t *stack[])
{
    calc_errno_t status = CALC_SUCCESS;
    
    assert(NULL != stack);
    assert(NULL != expression);
    
    while (!StackIsEmpty(stack[OP]) && CALC_SUCCESS == status)
    {
        status = Folding(stack);        
    }
    
    return status;
}

/*********** Push to op stack in case of opening bracket **********************/
static calc_errno_t PushBrackets(char **expression, stack_t *stack[])
{
    assert(NULL != stack);
    assert(NULL != expression);
    
    StackPush(stack[OP], *expression);

    ++(*expression);

    return CALC_SUCCESS;
}

/*********** Fold expression between brackets *********************************/
static calc_errno_t FoldBrackets(char **expression, stack_t *stack[])
{
    calc_errno_t status = CALC_SUCCESS;
    
    assert(NULL != expression);
    assert(NULL != stack);

    while ((!StackIsEmpty(stack[OP])) && (CALC_SUCCESS == status) &&
         ('(' != *(char *)StackPeek(stack[OP])))
    {
        status = Folding(stack);
    }

    if (StackIsEmpty(stack[OP]))
    {
        return CALC_ERR_INVALID_EXPRESSION;
    }
    
    StackPop(stack[OP]);
    ++(*expression);
    
    return status;
}

/*********** Addition function ************************************************/
calc_errno_t Addition(double left, double right, stack_t *stack[])
{
    double result = 0.0;
    int push_status = STACK_OVERFLOW_ERROR;
    
    assert(NULL != stack);
        
    result = left + right;
    
    push_status = StackPush(stack[NUM], &result);
    
    assert(STACK_OVERFLOW_ERROR != push_status);
    
    return CALC_SUCCESS;
}

/*********** Subtraction function *********************************************/
calc_errno_t Subtraction(double left, double right, stack_t *stack[])
{
    double result = 0.0;
    int push_status = STACK_OVERFLOW_ERROR;
    
    assert(NULL != stack);
        
    result = left - right;
    
    push_status = StackPush(stack[NUM], &result);
    
    assert(STACK_OVERFLOW_ERROR != push_status);
    
    return CALC_SUCCESS;
}

/*********** Multiplication function ******************************************/
calc_errno_t Multiply(double left, double right, stack_t *stack[])
{
    double result = 0.0;
    int push_status = STACK_OVERFLOW_ERROR;
    
    assert(NULL != stack);
        
    result = left * right;
    
    push_status = StackPush(stack[NUM], &result);
    
    assert(STACK_OVERFLOW_ERROR != push_status);
    
    return CALC_SUCCESS;
}

/*********** Division function ************************************************/
calc_errno_t Division(double left, double right, stack_t *stack[])
{
    double result = 0.0;
    int push_status = STACK_OVERFLOW_ERROR;
    
    assert(NULL != stack);
    
    if (0 == right)
    {
        return CALC_ERR_INVALID_CALCULATION;
    }
        
    result = left / right;
    
    push_status = StackPush(stack[NUM], &result);
    
    assert(STACK_OVERFLOW_ERROR != push_status);
    
    return CALC_SUCCESS;
}

/*********** Exponentiation function ******************************************/
calc_errno_t Exponentiation(double left, double right, stack_t *stack[])
{
    double result = 0.0;
    int push_status = STACK_OVERFLOW_ERROR;
    
    assert(NULL != stack);

    if(left == 0 && right <= 0)
    {
        return CALC_ERR_INVALID_CALCULATION;
    }
    
    else if (left < 0 && floor(right) != right)
    {
        return CALC_ERR_INVALID_CALCULATION;
    }
    
    result = pow(left, right);
    
    push_status = StackPush(stack[NUM], &result);
    
    assert(STACK_OVERFLOW_ERROR != push_status);
    
    return CALC_SUCCESS;
}

/*********** Create num stack and op stack ************************************/
static calc_errno_t LUTStackCreateInit(const char *expression)
{
    stack_t *num_stack = NULL;
    stack_t *op_stack = NULL;
    size_t expression_length = 0;
    
    assert(NULL != expression);
    
    expression_length = strlen(expression) + 1;
    
    num_stack = StackCreate(expression_length, sizeof(double));
    if (NULL == num_stack)
    {
        return CALC_FAILED_ALOC;
    }
    
    op_stack = StackCreate(expression_length, sizeof(char));
    if (NULL == op_stack)
    {
        StackDestroy(num_stack);        
        return CALC_FAILED_ALOC;
    }
       
    LUT_STACK[NUM] = num_stack;
    LUT_STACK[OP] = op_stack;
    
    return CALC_SUCCESS;
} 

/*********** Destroy num stack and op stack ************************************/
static void LUTStackDestroy(stack_t *stack[])
{
    StackDestroy(stack[NUM]);
    StackDestroy(stack[OP]);
}

/*********** initiate FSM *****************************************************/
static void FSMInit()
{
    int i = 0;
    
    /* initiate all LUT elements to error function */
    for (i = 0; i < ASCII_SIZE; ++i)
    {
        CALC_FSM[i][WAIT_FOR_NUM].action_func = &ErrorInvalidExp;
        CALC_FSM[i][WAIT_FOR_NUM].next_state = ERROR;
        CALC_FSM[i][WAIT_FOR_OP].action_func = &ErrorInvalidExp;
        CALC_FSM[i][WAIT_FOR_OP].next_state = ERROR;
    }
    
    /* initiate digit functions in wait for num state */
    for (i = '0'; i <= '9'; ++i)
    {
        CALC_FSM[i][WAIT_FOR_NUM].action_func = &PushNumToStack;
        CALC_FSM[i][WAIT_FOR_NUM].next_state = WAIT_FOR_OP;
    }
    
    /*initiate index functions that does not change the state */
    CALC_FSM[' '][WAIT_FOR_NUM].action_func = &IgnoreWhiteSpace;
    CALC_FSM[' '][WAIT_FOR_NUM].next_state = WAIT_FOR_NUM;
    CALC_FSM['\t'][WAIT_FOR_NUM].action_func = &IgnoreWhiteSpace;
    CALC_FSM['\t'][WAIT_FOR_NUM].next_state = WAIT_FOR_NUM;
    CALC_FSM['\n'][WAIT_FOR_NUM].action_func = &IgnoreWhiteSpace;
    CALC_FSM['\n'][WAIT_FOR_NUM].next_state = WAIT_FOR_NUM;
    CALC_FSM['\f'][WAIT_FOR_NUM].action_func = &IgnoreWhiteSpace;
    CALC_FSM['\f'][WAIT_FOR_NUM].next_state = WAIT_FOR_NUM;
    CALC_FSM['\v'][WAIT_FOR_NUM].action_func = &IgnoreWhiteSpace;
    CALC_FSM['\v'][WAIT_FOR_NUM].next_state = WAIT_FOR_NUM;
    CALC_FSM['\r'][WAIT_FOR_NUM].action_func = &IgnoreWhiteSpace;
    CALC_FSM['\r'][WAIT_FOR_NUM].next_state = WAIT_FOR_NUM;
    CALC_FSM['('][WAIT_FOR_NUM].action_func = &PushBrackets;
    CALC_FSM['('][WAIT_FOR_NUM].next_state = WAIT_FOR_NUM;            

    /*initiate functions in wait for num state and transition to next state */
    CALC_FSM['.'][WAIT_FOR_NUM].action_func = &PushNumToStack;
    CALC_FSM['.'][WAIT_FOR_NUM].next_state = WAIT_FOR_OP;
    CALC_FSM['+'][WAIT_FOR_NUM].action_func = &CheckUnary;
    CALC_FSM['+'][WAIT_FOR_NUM].next_state = WAIT_FOR_OP;
    CALC_FSM['-'][WAIT_FOR_NUM].action_func = &CheckUnary;
    CALC_FSM['-'][WAIT_FOR_NUM].next_state = WAIT_FOR_OP;

    /*initiate functions in wait for op state and transition to next state */    
    CALC_FSM['*'][WAIT_FOR_OP].action_func = &PushOperatorToStack;
    CALC_FSM['*'][WAIT_FOR_OP].next_state = WAIT_FOR_NUM;
    CALC_FSM['/'][WAIT_FOR_OP].action_func = &PushOperatorToStack;
    CALC_FSM['/'][WAIT_FOR_OP].next_state = WAIT_FOR_NUM;
    CALC_FSM['^'][WAIT_FOR_OP].action_func = &PushOperatorToStack;
    CALC_FSM['^'][WAIT_FOR_OP].next_state = WAIT_FOR_NUM;
    CALC_FSM['+'][WAIT_FOR_OP].action_func = &PushOperatorToStack;
    CALC_FSM['+'][WAIT_FOR_OP].next_state = WAIT_FOR_NUM;
    CALC_FSM['-'][WAIT_FOR_OP].action_func = &PushOperatorToStack;
    CALC_FSM['-'][WAIT_FOR_OP].next_state = WAIT_FOR_NUM;
    CALC_FSM['\0'][WAIT_FOR_OP].action_func = &FoldStack;
    CALC_FSM['\0'][WAIT_FOR_OP].next_state = END;
    
    /*initiate index op functions that does not change the state */    
    CALC_FSM[' '][WAIT_FOR_OP].action_func = &IgnoreWhiteSpace;
    CALC_FSM[' '][WAIT_FOR_OP].next_state = WAIT_FOR_OP;
    CALC_FSM['\t'][WAIT_FOR_OP].action_func = &IgnoreWhiteSpace;
    CALC_FSM['\t'][WAIT_FOR_OP].next_state = WAIT_FOR_OP;
    CALC_FSM['\n'][WAIT_FOR_OP].action_func = &IgnoreWhiteSpace;
    CALC_FSM['\n'][WAIT_FOR_OP].next_state = WAIT_FOR_OP;
    CALC_FSM['\r'][WAIT_FOR_OP].action_func = &IgnoreWhiteSpace;
    CALC_FSM['\r'][WAIT_FOR_OP].next_state = WAIT_FOR_OP;
    CALC_FSM['\f'][WAIT_FOR_OP].action_func = &IgnoreWhiteSpace;
    CALC_FSM['\f'][WAIT_FOR_OP].next_state = WAIT_FOR_OP;
    CALC_FSM['\v'][WAIT_FOR_OP].action_func = &IgnoreWhiteSpace;
    CALC_FSM['\v'][WAIT_FOR_OP].next_state = WAIT_FOR_OP;
    CALC_FSM[')'][WAIT_FOR_OP].action_func = &FoldBrackets;
    CALC_FSM[')'][WAIT_FOR_OP].next_state = WAIT_FOR_OP;
}

/*********** initiate operator LUT ********************************************/
static void OperatorLUTInit()
{
    LUT_OPERATOR['+'].operation_func = &Addition;  
    LUT_OPERATOR['+'].precedence = 1;
    LUT_OPERATOR['-'].operation_func = &Subtraction;  
    LUT_OPERATOR['-'].precedence = 1;
    LUT_OPERATOR['*'].operation_func = &Multiply;  
    LUT_OPERATOR['*'].precedence = 2;
    LUT_OPERATOR['/'].operation_func = &Division;  
    LUT_OPERATOR['/'].precedence = 2;
    LUT_OPERATOR['('].precedence = 0;
    LUT_OPERATOR[')'].precedence = 0;
    LUT_OPERATOR['^'].operation_func = &Exponentiation;  
    LUT_OPERATOR['^'].precedence = 3;
}


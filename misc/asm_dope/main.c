/*++

\author Daulet Tumbayev
\brief Example of assembly dope into C/C++
\env User mode

--*/

//------------------------------------------------------------------------
//  Includes.
//------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------
//  Functions for example
//------------------------------------------------------------------------

/**
 * \brief example of function compiled from C\C++ source code
 * \param a term1
 * \param b term2
 * \return sum of two numbers
 */
#ifdef __cplusplus
extern "C"
#endif
int _fastcall func_plus(int a, int b) {
	return a + b;
}

/**
 * \brief example of function compiled from ASM source code
 * \param a term1
 * \param b term2
 * \return difference of two numbers
 */
#ifdef __cplusplus
extern "C"
#endif 
int _fastcall func_minus(int a, int b);

/**
 * \brief example of function compiled from ASM source code
 * \param a term1
 * \param b term2
 * \return multiplication of two numbers
 */
#ifdef __cplusplus
  extern "C"
#endif 
int _fastcall func_mul(int a, int b);

//------------------------------------------------------------------------
//  Structs and enums.
//------------------------------------------------------------------------

/**
 * \brief to undestand which sign we recieved from program start input
 */
typedef enum _SIGN
{
	ERROR,
	PLUS,
	MINUS,
	MUL
} SIGN, *PSIGN;

//------------------------------------------------------------------------
//  Local functions
//------------------------------------------------------------------------

/**
 * \brief just run instruction
 */
void print_help()
{
  printf("input error\n");
  printf("Usage:\n");
  printf("\t asm_dope.exe term1 sign term2\n");
  printf("\t\t where: terms must be positive integer, sign must be '+', '-' or '*'.\n");
  printf("\t\t for example: asm_dope.exe 2 + 3\n");
}

/**
 * \brief processing input arguments
 * \param argc amount of arguments
 * \param argv arguments
 * \param number1 placeholder for first term
 * \param number2 placeholder for second term
 * \return sign of the operation
 */
SIGN process_input(int argc, char* argv[], int* number1, int* number2)
{
  SIGN sign;

  if (argc != 4 || !number1 || !number2)
  {
    return ERROR;
  }
  
  switch (argv[2][0]) // first simbol of sign string
  {
  case '+':
    sign = PLUS;
    break;
  case '-':
    sign = MINUS;
    break;
  case '*':
    sign = MUL;
    break;
  default:
    sign = ERROR;
    break;
  }
  
  *number1 = atoi(argv[1]);
  *number2 = atoi(argv[3]);

  return sign;
}

//------------------------------------------------------------------------
//  Entry point
//------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	int ret = -1;
  int number1 = 0;
  int number2 = 0;
  SIGN sign;

  sign = process_input(argc, argv, &number1, &number2);

  if (ERROR == sign)
  {
    print_help();
    return ret;
  }

  switch (sign)
  {
  case PLUS:
    ret = func_plus(number1, number2);
    break;
  case MINUS:
    ret = func_minus(number1, number2);
    break;
  case MUL:
    ret = func_mul(number1, number2);
    break;
  default:
    print_help();
    break;
  }

  return ret;
}


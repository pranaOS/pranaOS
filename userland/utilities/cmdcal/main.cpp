#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int CheckArgumentCount(int argc);
int CheckOperands(char* operand);
long Calculate(long nr1, long nr2, char operation);

int main(int argc, char* argv[])
{
  if (!CheckArgumentCount(argc))
    exit(1);

  long nr1 = CheckOperands(argv[1]);
  long nr2 = CheckOperands(argv[3]);
  int result;


  result = Calculate(nr1, nr2, argv[2][0]);
  printf("Result: %d", result);

  return 0;
}


int CheckArgumentCount(int argc)
{
  if (argc != 4)
  {
    puts("Wrong number of arguments to perform the calculation.\n");
    return 0;
  }

  return 1;
}

int CheckOperands(char* operand)
{
  int i = 0;

  while (operand[i] != '\0')
  {
    if (operand[i] < '0' || operand[i] > '9')
    {
      return operand[i];
    }

    i++;
  }
  return atoi(operand);
}


long Calculate(long nr1, long nr2, char operation)
{
  long result = 0;

  switch (operation)
  {
  case '+':
    result = nr1 + nr2;
    break;
  case '-':
    result = nr1 - nr2;
    break;
  case '*':
    result = nr1 * nr2;
    break;
  case '/':
    if (nr2 == 0)
    {
      puts("Error! Divion by zero.");
    }
    else
    {
      result = nr1 / nr2;
    }
    break;
  default:
    puts("Operator invalid.");
    break;
  }
  return result;
}

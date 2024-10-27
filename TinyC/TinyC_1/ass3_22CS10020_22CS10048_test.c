#include <stdio.h>

int main()
{
  // Sample integer constants
  int a = 123;
  int b = 0;
  int c = -456;

  // Sample floating-point numbers
  float x = 1.23;
  float y = -0.456;
  float z = 7.89e+10;
  float w = 4.56E-3;

  // Sample identifiers
  int sum = 0;
  float average = 0.0;
  char ch = 'A';

  // Sample keywords and control structures
  if (a > b)
  {
    sum = a + b;
  }
  else
  {
    sum = b - a;
  }

  for (int i = 0; i < 10; i++)
  {
    average += i;
  }
  average /= 10;

  // Sample comments
  // This is a single-line comment

  /*
      This is a multi-line comment
      spanning multiple lines.
  */

  // Sample operators and punctuation
  int d = a * b;
  int e = c / d;
  int f = a % b;
  int g = a && b;
  int h = a || b;
  int i = a & b;
  int j = a | b;
  int k = a ^ b;
  int l = ~a;
  int m = a << 2;
  int n = b >> 3;

  // Sample strings
  char *str = "Hello, World!";

  // Sample exponent parts
  double exp1 = 2e10;
  double exp2 = 5.12E-3;
  double exp3 = 1.23e+7;

  printf("Sum: %d\n", sum);
  printf("Average: %.2f\n", average);
  printf("String: %s\n", str);
  printf("Exponent 1: %f\n", exp1);
  printf("Exponent 2: %f\n", exp2);
  printf("Exponent 3: %f\n", exp3);
  return 0;
}
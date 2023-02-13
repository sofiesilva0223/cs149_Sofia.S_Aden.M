/**
 * Description: Prompts use to enter three floating-point numbers x, y, and z and prints x to the power of z, x to the power of (y to the power of 2), the absolute value of y, and the square root of (xy to the power of z). 
 * Author names: Sofia Silva & Aden Mengistie 
 * Author emails: sofia.silva@sjsu.edu, aden.mengistie@sjsu.edu
 * Last modified date: 02/08/2023
 * Creation date: 02/08/2023
 **/
 
#include <stdio.h>
#include <math.h>

int main(void) {
   double x;
   double y;
   double z;

   scanf("%lf", &x);
   scanf("%lf", &y);
   scanf("%lf", &z);
   
   printf("%0.2lf %0.2lf %0.2lf %0.2lf\n", pow(x,z), pow(x,pow(y,2)), fabs(y), sqrt(pow((x*y),z)));

   return 0;
}


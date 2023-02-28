/**
 * Description:A function named LapsToMiles takes a double as a parameter, representing the number of laps, and returns a double that represents the number of miles. The main program takes a number of laps as an input, calls function LapsToMiles() to calculate the number of miles, and outputs the number of miles.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 2/26/2023
 * Creation date: 2/26/2023
 **/
#include <stdio.h>

/* Define your function here */ 
double LapsToMiles(double userLaps);

int main(void) {
   /* Type your code here. Your code must call the function.  */
   double userLaps;
   scanf("%lf", &userLaps);
   printf("%0.2lf\n", LapsToMiles(userLaps));
   return 0;
}
double LapsToMiles(double userLaps){
   return userLaps * 0.25;
}


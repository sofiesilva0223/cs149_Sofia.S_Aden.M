/**
 * Description: User enters two integers: current price and last month's price. Prints a summary listing the price, change since last month and the estimated monthly mortgage.
 * Author names: Sofia Silva & Aden Mengistie 
 * Author emails: sofia.silva@sjsu.edu, aden.mengistie@sjsu.edu
 * Last modified date: 02/08/2023
 * Creation date: 02/08/2023
 **/
 
 #include <stdio.h>

int main(void) {
   int currentPrice;
   int lastMonthsPrice;
   
   scanf("%d", &currentPrice);
   scanf("%d", &lastMonthsPrice);
   
   printf("%s%d%s%d %s\n","This house is $",currentPrice,". The change is $",currentPrice-lastMonthsPrice,
      "since last month.");
   printf("%s%0.6lf%s\n","The estimated monthly mortgage is $",(currentPrice * 0.051) / 12,".");

   return 0;
}


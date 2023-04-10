/**
 * Description: Contains main.c - main() function where the program prompts the user for two items and create two objects of the ItemToPurchase struct. The output is the sum of the two items.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 04/09/2023
 * Creation date: 04/09/2023
 **/

#include<stdio.h>
#include<string.h>

#include "ItemToPurchase.h"

int main(void) {
 
   struct ItemToPurchase item1;
   struct ItemToPurchase item2;
   int total=0;
   char c;
   
   MakeItemBlank(&item1);
   MakeItemBlank(&item2);
   
   printf("Item 1\n");
   
   printf("Enter the item name:\n");
   scanf("%[^\n]",item1.itemName);
   
   printf("Enter the item price:\n");      
   scanf("%d",&item1.itemPrice);
   
   printf("Enter the item quantity:\n");
   scanf("%d",&item1.itemQuantity);
   
   c = getchar();
   while(c != '\n' && c != EOF) {
      c = getchar();
   }
   printf("\nItem 2\n");
   
   printf("Enter the item name:\n");
   scanf("%[^\n]",item2.itemName);
   
   printf("Enter the item price:\n");      
   scanf("%d",&item2.itemPrice);
   
   printf("Enter the item quantity:\n");
   scanf("%d",&item2.itemQuantity);
   
   printf("\n");
   printf("TOTAL COST\n");
   PrintItemCost(&item1);
   PrintItemCost(&item2);
   printf("\n");
   
   total = item1.itemPrice *item1.itemQuantity + item2.itemPrice* item2.itemQuantity;
   printf("Total: $%d\n",total);
   return 0;
}

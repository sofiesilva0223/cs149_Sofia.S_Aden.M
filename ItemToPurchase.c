/**
 * Description: Contains ItemToPurchase.c - Related function definitions
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 04/09/2023
 * Creation date: 04/09/2023
 **/

#include "ItemToPurchase.h"

void MakeItemBlank(struct ItemToPurchase *item){
   strcpy(item->itemName,"none");
   item->itemPrice=0;
   item->itemQuantity=0;
}
void PrintItemCost(struct ItemToPurchase *item){
   printf("%s %d @ $%d = $%d\n",item->itemName,item->itemQuantity,item->itemPrice,item->itemQuantity*item->itemPrice);
}

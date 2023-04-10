/**
 * Description: Contains ItemToPurchase.h - Struct definition and related function declarations
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 04/09/2023
 * Creation date: 04/09/2023
 **/
 
#ifndef ITEMTOPURCHASE_H
#define ITEMTOPURCHASE_H

#include<string.h>
#include<stdio.h>

typedef struct ItemToPurchase{
   char itemName[64];
   int itemPrice;
   int itemQuantity;
}ItemToPurchase;
void MakeItemBlank(struct ItemToPurchase *item);
void PrintItemCost(struct ItemToPurchase *item);
#endif

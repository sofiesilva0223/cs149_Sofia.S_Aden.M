/**
 * Description: Program reads the input the name and phone number for three contacts and output each contact. Three ContactNodes were created and used nodes to build a linked list.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 04/17/2023
 * Creation date: 04/16/2023
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ContactNode{
   char contactName[60];
   char contactPhoneNumber[60];
   struct ContactNode* nextNodePtr; //pointer
} ContactNode;

ContactNode* headNode = NULL;
/* Define the functions here. */

//initializing node
void InitializeContactNode(ContactNode* node,char name[], char phoneNum[]){
   strcpy(node->contactName, name);
   strcpy(node->contactPhoneNumber, phoneNum);
   node -> nextNodePtr = NULL;
}
//getter for contactName
char* GetName(ContactNode* node){   
   return node -> contactName;
}
//getter for contactPhoneNumber
char* GetPhoneNumber(ContactNode* node){
   return node -> contactPhoneNumber;
}

// ***Linked List Operations*** //
void InsertAfter(ContactNode* current, ContactNode* node){
   node -> nextNodePtr = current -> nextNodePtr;
   current -> nextNodePtr = node;
}   
ContactNode* GetNext(ContactNode* node){
   return node -> nextNodePtr;
}
void PrintContactNode(ContactNode* node){
   printf("Name: %s\nPhone number: %s\n", node -> contactName,node -> contactPhoneNumber);
      
}

int main(void) {
   char name[60] = "";
   char phoneNum[60] = "";
   ContactNode* insertLoc = NULL;

   // Get contact information 
   for(int i = 1; i <= 3; i++){
      printf("Person %d: ", i);
      
      fgets(name, 60, stdin); //read name
      name[strlen(name)-1] = '\0';

      fgets(phoneNum, 60, stdin); //read phone number
      phoneNum[strlen(phoneNum)-1] = '\0';
      
      printf("%s, %s\n",name,phoneNum);
      
      // use malloc to allocate nextContact1
      ContactNode* node = (ContactNode*)malloc(sizeof(ContactNode));
      InitializeContactNode(node,name, phoneNum);
      
      if(headNode == NULL){
         headNode = node;
         insertLoc = headNode;
      }
      else{
         InsertAfter(insertLoc,node);
         insertLoc = node;
      }
   }

   printf("\nCONTACT LIST\n");
   // Print contact list
   // initialize currContact to headContact
   ContactNode* current = headNode;
   // traverse nodes with currContact until == NULL
   while(current != NULL)
   {
      // use PrintContactNode to print out each node
      PrintContactNode(current);
      printf("\n");
      // get the contact next to currContact
      current = GetNext(current);
   }

   return 0;
}


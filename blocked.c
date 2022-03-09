// File : Blocked_.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "blocked.h"

//===============================================
void Blocked_InitEmpty(Blocked * self)
{
	self -> capacity = 0;
  self -> length = 0;
	self -> mailAddress = NULL;
}

//===============================================
bool Blocked_IsFull(Blocked self) 
{
  return self.length == self.capacity;
}

//===============================================
void Blocked_Realloc(Blocked * self)
{
  
 if (self -> capacity== 0){
   int newCapacity = 2;
   self -> mailAddress = realloc(self->mailAddress, sizeof(newCapacity) * self->length);
 }
 
 int doubleCapacity = 2 * self -> capacity;
 self -> mailAddress = realloc(self ->mailAddress, sizeof(doubleCapacity) * self->length);
 
}

//===============================================
void Blocked_AddAddress(Blocked * self, BlockedAddress mailAddress)
{
  if (Blocked_IsFull(*self)){
    Blocked_Realloc(self);
    Blocked_AddAddress(self,mailAddress);
  }
  strcpy(&mailAddress [self->length], mailAddress);
}

//===============================================
void Blocked_AddAddressFromFile(Blocked * self, char * filename)
{
  FILE * end;
  end = fopen(filename, "r+");
  char c[1024];
  
  if (end != NULL){
    while (fgets(c, 1024, end) != NULL){
    Blocked_AddAddress(self,c);
      }
  }
}

//===============================================
bool Blocked_IsBlocked(Blocked self, BlockedAddress mailAddress)
{
 for (int i = 0; i < self.capacity; i++){
   if (strcmp(mailAddress, *self.mailAddress) == 0){
     return true;}
 }
}

//===============================================
void Blocked_Destroy(Blocked * self)
{
  free(self->mailAddress);
  Blocked_InitEmpty(self);
}

// File : mail.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "mail.h"

//===============================================
void Mail_InitEmpty(Mail * self)
{
	self->capacity = 0;
  self->length = 0;
  self->sender = NULL;
	self->subject = NULL;
	self->classification = NULL;
	self->totalWeight = NULL;
	self->filename = NULL;
}

//===============================================
bool Mail_IsFull(Mail self)
{
  return self.length == self.capacity;
}

//===============================================
void Mail_Realloc(Mail * self)
{
  if (self->capacity == 0)
    self->capacity = 1;

  int newCapacity = self->capacity * 2;
  MailAddress * newSender = realloc(self->sender, newCapacity * sizeof * newSender);
  assert (newSender != NULL );
  Subject * newSubject = realloc(self->subject, newCapacity * sizeof * newSubject);
  assert (newSubject != NULL );
  MailClassification * newClassification = realloc(self->classification, newCapacity * sizeof * newClassification);
  assert (newClassification != NULL );
  int * newTotalWeight = realloc(self->totalWeight, newCapacity * sizeof * newTotalWeight);
  assert (newTotalWeight != NULL );
  Filename * newFilename = realloc(self->filename, newCapacity * sizeof * newFilename);
  assert (newFilename != NULL );

  self->capacity = newCapacity;
  self->sender = newSender;
  self->subject = newSubject;
  self->classification = newClassification;
  self->totalWeight = newTotalWeight;
  self->filename = newFilename;
}

//===============================================
bool Mail_IsMail(FILE * fin)
{
  //FILE * end;
  char c[1024];
  
  if (fin != NULL){
    fgets(c, 1024, fin);
      return (strcmp(c, "#email") == 0);
  }
  return false;
}

//===============================================
void Mail_GetSender(FILE * fin, MailAddress sender)
{
  fscanf(fin, "%s\n", sender);
}

//===============================================
void Mail_GetSubject(FILE * fin, Subject subject)
{
  fgets(subject, MAX_SUBJECT_SIZE, fin);
}

//===============================================
void Mail_AddMail(Mail * self, Filename filename)
{
  FILE *fin;
  fin = fopen(filename, "r");
  if (fin == NULL){
    printf("le fichier n'a pas pu etre ouvert");
    exit(0);
  
  MailAddress sender;
  Subject subject;
  
  fscanf(fin, "%s\n", sender);
  fgets(subject, MAX_SUBJECT_SIZE, fin);
   
 }
}

//===============================================
void Mail_AddMailsFromDirectory(Mail * self, char * dirName)
{

	DIR *rep ;
	struct dirent *file ;
	struct stat sb;


	rep = opendir (dirName) ;
	if (rep != NULL)       {           //l'ouverture a reussit
		while (file = readdir(rep))      //lecture du repertoire
 
			if( stat(dirName, &sb) != -1) {
				if( S_ISREG( sb.st_mode ) != 0 ) {   //le fichier est regulier
					Mail_AddMail(self, file->d_name);

				}
			}
closedir(rep);
		}
}



//===============================================
bool Mail_IsBlocked(Mail self, int index)
{
  return self.classification[index] == BLOCKED;
}

//===============================================
bool Mail_IsSuspected(Mail self, int index)
{
  return self.classification[index] == SUSPECTED;
}

//===============================================
bool Mail_IsClean(Mail self, int index)
{
  return !(Mail_IsBlocked(self, index) || Mail_IsSuspected(self, index));
}

//===============================================
void Mail_Classify(Mail *self, int index, Blocked blocked, Suspected suspected)
{

	if (Blocked_IsBlocked(blocked, self->sender[index])){
		self -> classification[index] = BLOCKED;
		
		}
		
		
	if( Suspected_IsSuspected(suspected, self->subject[index], 60)){
		self -> classification[index] = SUSPECTED;
		}
}

//===============================================
void Mail_ClassifyAll(Mail *self, Blocked blocked, Suspected suspected)
{
  for (int i=0; i<self->length; i++){
    Mail_Classify(self, i, blocked, suspected);
  }
}

//===============================================
void Mail_Rename(Mail self, int index)
{
	
	char * a = "[BLOCKED]";
	char * b = "[SUSPECTED ";
		

	char c[1024];
														// char * itoa(int num, char * buffer, int base)  
																																							
	if (Mail_IsBlocked(self,index)){
		strcat(self.filename[index], a);
		sprintf(c, "%s[BLOCKED]", self.filename);
		
		rename(self.filename[index], c);
		}
	
	if (Mail_IsSuspected (self, index)){
		strcat(self.filename, c);
		sprintf(c, "%s[SUSPECTED %d]", self.filename, self.totalWeight);
		
		rename(self.filename[index], c);
	
		}

}

//===============================================
void Mail_RenameAll(Mail self)
{
  for (int i=0; i<self.length; i++){
    Mail_Rename(self, i);
  }
}

//===============================================
void Mail_Print(Mail self)
{
  for (int i=0; i<self.length; i++){
    printf ("Sender : %s\nSubject : %s\n", self.sender[i], self.subject[i]);
  }
}

//===============================================
void Mail_Destroy(Mail *self)
{
  free(self->sender);
  free(self->subject);
  free(self->classification);
  free(self->totalWeight);
  free(self->filename);
  Mail_InitEmpty(self);
}

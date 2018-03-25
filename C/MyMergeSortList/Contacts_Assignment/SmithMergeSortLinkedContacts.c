/*
 ============================================================================
 Name        : ContactsAndMemory.c
 Author      : Chad Mello
 Version     :
 Copyright   : Your copyright notice
 Description : Working with large files, data structures, and malloc, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dates.h"

 /* structure to hold contact data record */
typedef struct contacts
{
  char firstName[26];
  char lastName[31];
  date dob;
  date anniversary;
  char company[76];
  char street1[51];
  char city[51];
  char county[51];
  char state[3];
  char zip[13];
  char phone1[16];
  char phone2[16];
  char email[101];
  char web[101];
  struct contact * next;
} contact; //608 bytes

/* function prototypes */
unsigned int fGetLine(FILE *fps, char *line, int max_len);
void setString(char *dest, char *value);
contact * loadContactCsv(char* sourceFile, long maxRecords, unsigned long *recordsRead);
int parseContact(char * line, contact * record);
void printContactRecord(contact *record);
int writeToBinary(char* fileName, contact *contacts, size_t numRecords);
contact * loadFromBinary(char* sourceFile, long maxRecords, unsigned long *recordsRead);
void freeContacts(contact * head);

//Merge Sort Functions
void doMergeSort(contact ** head, int sortBy, int sortOrder);
void splitList(contact * head, contact ** h1, contact ** h2);
contact *mergeListsByLastName(contact *l1, contact *l2, int sortOrder);
contact *mergeListsByFirstName(contact *l1, contact *l2, int sortOrder);

//List Printing Functions
void printContactNames(contact * head, int start, long unsigned amount);
void printContactName(contact *record);
contact *moveToContact(contact * head, long unsigned pos);

/* import contacts from large text file,
* and write into a new binary file */
int main(void)
{
  unsigned long recordCount = 0;

  printf("Reading 1000000 records from binary file...\n", recordCount);
  contact * contactList = loadFromBinary("contacts.dat", 1000000, &recordCount);
  printf("Finished reading from binary file\n");

  if (contactList == NULL || recordCount == 0)
  {
    puts("Loading contact data failed. Goodbye.");
    exit(EXIT_FAILURE);
  }

  printf("There are %lu contacts in our array!\n", recordCount);
  printf("Each contact consists of %lu bytes.\n", sizeof(contact));

  printf("\nPrinting from 500,000, 15 entries.\n");
  contact* unsortedRefHead = contactList;
  printContactNames(unsortedRefHead, 500000, 15);
  printf("\n");

  contact* sortedRefHead = contactList;
  printf("\n********* UNSORTED LIST BELOW *********\n");
  printContactNames(sortedRefHead, 0, 10);

  /*A note to the grader: sorting technically works in terms of reordering pointers, but I have an issue somewhere in my mergesort that appears to cause lost data.
      I've noted the marked numbers that these lists seem to terminate at, but I can't otherwise pinpoint the exact issue at the moment.
      Data appears to be handled correctly before sorting, I suspect it may be an issue in my methods for merging the lists back together as
      the length of the list varies based loosely on what type of sorting method you use.*/


  //doMergeSort(&sortedRefHead, 0, 0); //Faults at 9
  doMergeSort(&sortedRefHead, 1, 0); //Faults at 11
  //doMergeSort(&sortedRefHead, 0, 1); //Faults at 12
  //doMergeSort(&sortedRefHead, 1, 1); //Faults at 8

  printf("\n********* SORTED LIST BELOW *********\n");
  printContactNames(sortedRefHead, 0, 20);
  printf("\nDone Printing.\n");

  // free the memory used to store the contacts
  freeContacts(contactList);
  printf("Completed Operations. Press any key to exit.\n");
  getch();
  return EXIT_SUCCESS;
}

/*writes contact records to binary file*/
int writeToBinary(char* fileName, contact *contacts, size_t numRecords)
{
  // SUPPLY YOUR CODE FOR THIS FUNCTION
  FILE *ptr;

  // -- fileName: holds the name of the file you must create
  //   			(this means that you will open a new file that will
  //    			overwrite the file if it already exists)

  // -- contacts: is a pointer to memory where all the contacts are currently stored

  // -- numRecords: holds the value telling you how many records/structs in memory
  //               (chunks) to write into the file

  //Function should return 1, if it is successful

  //(1) You need to open the file for write as binary (overwriting any existing file of same name)
  ptr = fopen(fileName, "wb");
  if (!ptr)
  {
    perror(fileName);
    return 0;
  }
  //(2) You then must write the data from memory location "contacts" into the file
  //    Note: numRecords tells you how many "Chunks" to write
  //fwrite(contacts, sizeof(contact), numRecords, ptr);
  contact * tracker = contacts;
  while ((tracker->next) != 0xCDCDCDCD) {
    //printf("%s\n", tracker->firstName);
    fwrite(tracker, sizeof(contact), 1, ptr);
    tracker = tracker->next;
  }

  //(3) Close the file (don't forget to flush first)
  fflush(ptr);
  fclose(ptr);
  //(4) Return 1 to signify success
  return 1;
}

/*loads contact records from binary file*/
contact * loadFromBinary(char* sourceFile, long maxRecords, unsigned long *recordsRead)
{
  unsigned long recsToRead = 0;
  *recordsRead = 0;

  // open binary file
  FILE *fps = fopen(sourceFile, "rb");

  if (fps != NULL) // make sure we have a valid file pointer
  {
    //calculate number of records in the file
    fseek(fps, 0L, SEEK_END);
    //set number of records to read
    recsToRead = ftell(fps) / sizeof(contact);
    rewind(fps);
  }
  else
  {
    // there was some sort of error
    perror(sourceFile);
    return NULL; //return null to indicate that a problem was encountered
  }

  //Don't read more than we were told...
  if (maxRecords > 0 && (recsToRead > maxRecords))
    recsToRead = maxRecords;

  //read the head first
  contact *head = malloc(sizeof(contact));
  if (head == NULL)
  {
    // there was some sort of error
    // we could not allocate enough memory to hold our contact data
    perror("Can't allocate memory for head.");
    return NULL; //return null to indicate that a problem was encountered
  }
  else
    fread(head, sizeof(contact), 1, fps);

  //Set current contact to be the head
  contact *curContact = head;
  *recordsRead = 1; // we have one record loaded so far..

  for (int i = 0; i < (recsToRead - 1); i++)
  {
    // allocate memory to hold next contact
    curContact->next = (contact *)malloc(sizeof(contact));

    if (curContact->next == NULL)
    {
      // There was some sort of error.
      // We could not allocate enough memory to hold our contact data
      perror("Can't allocate enough memory to load all contacts.");
      return head; // return the list we loaded up until now
    }

    //now, read all next contact into memory...
    fread(curContact->next, sizeof(contact), 1, fps);

    if (curContact->next == NULL)
      break;

    (*recordsRead)++;

    // Switch over the current contact to be
    // the latest contact loaded into memory.
    curContact = curContact->next;
  }
  curContact->next = NULL;
  fclose(fps);
  return head;
}


/* sends contact information to consol */
void printContactRecord(contact *record)
{
  // SUPPLY YOUR CODE FOR THIS FUNCTION
  if (record != 0xCDCDCDCD) {
    printf("%s, %s\n", record->lastName, record->firstName);
  }
}

/* sets value into a char array */
void setString(char *dest, char *value)
{
  strcpy(dest, value);
}

/* Reads an entire line from a text file.
Returns how many characters are in the line*/
unsigned int fGetLine(FILE *fps, char *line, int max_len)
{
  int tempChar;
  unsigned int tempCharIdx = 0U;

  /* get a character from the file pointer */
  while ((tempChar = fgetc(fps)))
  {
    /* avoid buffer overflow error */
    if (tempCharIdx == max_len)
    {
      fprintf(stderr, "Warning: line is too long.\n");
    }

    /* test character value */
    if (tempChar == EOF) // reached end of file?
    {
      line[tempCharIdx] = '\0';
      break;
    }
    else if (tempChar == '\n' // compensate for Windows CRLF or Linux LF
      || tempChar == '\r') {
      line[tempCharIdx] = '\0';
      break;
    }
    else
      if (tempCharIdx < max_len)
        line[tempCharIdx++] = (char)tempChar;
      else
      {
        // reached max size of line buffer - truncate the line
        line[tempCharIdx] = '\0';
        tempCharIdx++; // do not exit loop
                       //continue to read until end of line in file
                       // (even though the line is truncated)
      }
  }

  // return how many characters read
  return tempCharIdx;
}


/* Load contacts from a (text) comma delimited file*/
contact * loadContactCsv(char* sourceFile, long maxRecords, unsigned long *recordsRead)
{
  //pointer that holds access to head of contacts list
  contact *head;

  // allocate memory to hold head of contacts list
  head = (contact *)malloc(sizeof(contact));

  if (head == NULL)
  {
    // We could not allocate enough
    // memory to hold our contact data.
    perror("Can't allocate memory to import contacts.");
    return NULL;
  }

  // open csv file
  FILE *fps = fopen(sourceFile, "r");

  if (fps != NULL) // make sure we have a valid file pointer
  {
    char line[256]; // we can read a line with max 256 characters
                    // (including null terminator)

                    // Read each contact from the text file one line at a time.
                    // Note: 1 "record" = 1 line = 1 contact

                    //start with the head (first record in the file)...
    if (fGetLine(fps, line, sizeof(line)))
      parseContact(line, head);
    else
    {
      perror(sourceFile);
      perror("Could not load data from text file.");
      free(head);
      return NULL;
    }

    // now, continue to load the rest of
    // the contact list...
    contact *curContact = head;
    while (fGetLine(fps, line, sizeof(line)) > 0)
    {
      // allocate memory for the next contact
      curContact->next = (contact *)malloc(sizeof(contact));

      if (curContact->next == NULL)
      {
        // There was some sort of error (probably out of memory).
        // We could not allocate enough memory to hold our contact data
        perror("Can't allocate enough memory to import all contacts.");
        return head; // return the list we loaded up until now
      }

      // Place values we read from the file
      // into new contact record.
      parseContact(line, curContact->next);

      (*recordsRead)++; // increment list count

                        // Switch current contact to be the
                        // latest contact we loaded into memory.
      curContact = curContact->next;
    }

    // Ensure last element in
    // the list points to nothing.
    curContact->next = NULL;

    // close the file
    fclose(fps);

    // Return pointer to memory (the heap)
    // where head of the contacts list exists.
    return head;
  }
  else
  {
    // there was some sort of error
    perror(sourceFile);

    //free memory here
    if (head != NULL)
      free(head);

    return NULL; //return null to indicate that a problem was encountered
  }
}


/* parse fields from line and place values into contact record */
int parseContact(char * line, contact * record)
{
  // Now, parse the line to get
  // "field" values for the contact...
  //printf("Line: %s \n", line);

  const char s[2] = { ',' }; // separator used to tokenize each line we read from csv file
  char *token; // holds words between commas in each line

               /* get first name */
  token = strtok(line, s);
  if (token != NULL)
    setString(record->firstName, token);

  /* get last name */
  token = strtok(NULL, s);
  if (token != NULL)
    setString(record->lastName, token);

  /* get company name */
  token = strtok(NULL, s);
  if (token != NULL)
    setString(record->company, token);

  /* get street address */
  token = strtok(NULL, s);
  if (token != NULL)
    setString(record->street1, token);

  /* get city name */
  token = strtok(NULL, s);
  if (token != NULL)
    setString(record->city, token);

  /* get county name */
  token = strtok(NULL, s);
  if (token != NULL)
    setString(record->county, token);

  /* get state name */
  token = strtok(NULL, s);
  if (token != NULL)
    setString(record->state, token);

  /* get zip */
  token = strtok(NULL, s);
  if (token != NULL)
    setString(record->zip, token);

  /* get phone1 */
  token = strtok(NULL, s);
  if (token != NULL)
    setString(record->phone1, token);

  /* get phone2 */
  token = strtok(NULL, s);
  if (token != NULL)
    setString(record->phone2, token);

  /* get email */
  token = strtok(NULL, s);
  if (token != NULL)
    setString(record->email, token);

  /* get web */
  token = strtok(NULL, s);
  if (token != NULL)
    setString(record->web, token);

  //default dates to ZERO
  record->dob = ZERO_DATE;
  record->anniversary = ZERO_DATE;

  return 1;
}

void freeContacts(contact * head) {
  contact *tmp;
  while (head != NULL)
  {
    tmp = head;
    head = head->next;
    free(tmp);
  }
}

void doMergeSort(contact ** head, int sortBy, int sortOrder)
{
  if (head == NULL || *head == NULL || (*head)->next == NULL) {
    return;
  }

  contact *first;
  contact *second;

  //printf("Pre Split Head: %s\n", (*head)->firstName);
  splitList(*head, &first, &second);
  //printf("First Unit: %s %s, Second Unit: %s %s\n", (first)->firstName, (first)->lastName, (second)->firstName, (second)->lastName);
  doMergeSort(&first, sortBy, sortOrder);
  //printf("\nSwitching to second half.\n");
  doMergeSort(&second, sortBy, sortOrder);

  if (sortBy == 0) {
    *head = mergeListsByLastName(first, second, sortOrder);
  }
  else {
    *head = mergeListsByFirstName(first, second, sortOrder);
  }
}

void splitList(contact * head, contact ** h1, contact ** h2)
{
  if (head == NULL || head->next == NULL) {
    *h1 = head;
    *h2 = NULL;
    return;
  }

  contact* once = head;
  contact* twice = head->next;
  while (twice != NULL) {
    twice = (twice->next);
    if (twice != NULL) {
      twice = (twice->next);
      once = (once->next);
    }
  }
  *h1 = head;
  *h2 = once->next;
  once->next = NULL;
}

contact * mergeListsByLastName(contact * l1, contact * l2, int sortOrder)
{
  contact * retVal = NULL;
  //Ascending Sort
  if (sortOrder == 0) {
    if (strcmp(l1->lastName, l2->lastName) >= 0) {
      retVal = l2;
      l2 = l2->next;
    }
    else {
      retVal = l1;
      l1 = l1->next;
    }

    while (l1 && l2) {
      if (strcmp(l1->lastName, l2->lastName) >= 0) {
        retVal->next = l1;
        l1 = l1->next;
      }
      else {
        retVal->next = l2;
        l2 = l2->next;
      }
    }
  }
  else {
    if (strcmp(l1->lastName, l2->lastName) <= 0) {
      retVal = l2;
      l2 = l2->next;
    }
    else {
      retVal = l1;
      l1 = l1->next;
    }

    while (l1 && l2) {
      if (strcmp(l1->lastName, l2->lastName) <= 0) {
        retVal->next = l1;
        l1 = l1->next;
      }
      else {
        retVal->next = l2;
        l2 = l2->next;
      }
    }
  }
  //2 single components.
  if (l1 == NULL) {
    retVal->next = l2;
  }
  else if (l2 == NULL) {
    retVal->next = l1;
  }
  return retVal;
}

contact * mergeListsByFirstName(contact * l1, contact * l2, int sortOrder)
{
  contact * retVal = NULL;
  //Ascending Sort
  if (sortOrder == 0) {
    if (strcmp(l1->firstName, l2->firstName) >= 0) {
      retVal = l2;
      l2 = l2->next;
    }
    else {
      retVal = l1;
      l1 = l1->next;
    }

    while (l1 && l2) {
      if (strcmp(l1->firstName, l2->firstName) >= 0) {
        retVal->next = l1;
        l1 = l1->next;
      }
      else {
        retVal->next = l2;
        l2 = l2->next;
      }
    }
  }
  else {
    if (strcmp(l1->firstName, l2->firstName) <= 0) {
      retVal = l2;
      l2 = l2->next;
    }
    else {
      retVal = l1;
      l1 = l1->next;
    }

    while (l1 && l2) {
      if (strcmp(l1->firstName, l2->firstName) <= 0) {
        retVal->next = l1;
        l1 = l1->next;
      }
      else {
        retVal->next = l2;
        l2 = l2->next;
      }
    }
  }
  //2 single components.
  if (l1 == NULL) {
    retVal->next = l2;
  }
  else if (l2 == NULL) {
    retVal->next = l1;
  }
  return retVal;
}

void printContactNames(contact * head, int start, long unsigned amount)
{
  contact* ref = moveToContact(head, start);
  for (int i = 0; i < amount; i++) {
    if (ref != NULL) {
      printContactName(ref);
      ref = ref->next;
    }
    else {
      printf("Ref is null at %d!\n", i);
      break;
    }
  }
}

void printContactName(contact * record)
{
  printf("%s, %s\n", record->lastName, record->firstName);
}

contact * moveToContact(contact * head, long unsigned pos)
{
  contact* temp = head;
  for (int i = 0; i < pos; i++) {
    if (temp != NULL) {
      temp = temp->next;
    }
  }
  return temp;
}


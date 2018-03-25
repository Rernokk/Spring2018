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
void printContactRecord(contact *record);
unsigned int fGetLine(FILE *fps, char *line, int max_len);
void setString(char *dest, char *value);
contact * loadContactCsv(char* sourceFile, long maxRecords, unsigned long *recordsRead);
int parseContact(char * line, contact * record);
int writeContacts(char* fileName, contact *contacts, size_t numRecords);
contact * loadContacts(char* sourceFile, long maxRecords, unsigned long *recordsRead);
void freeContacts(contact * head);

//Merge Sort Prototypes
void doMergeSort(contact ** head, int sortBy, int sortOrder, int iter);
void splitList(contact * head, contact ** h1, contact **h2);
contact *mergeListsByLastName(contact *l1, contact *l2, int sortOrder);
contact *mergeListsByFirstName(contact * l1, contact *l2, int sortOrder);
void swapNodes(contact **node1, contact **node2);

/* import contacts from large text file,
* and write into a new binary file */
int main(void)
{
  unsigned long recordCount = 0;

  /*
  //printf("Loading contacts from csv.  Please wait...\n");

  //// load contacts from text-based csv file.
  //contact * contacts = loadContactCsv("us-1000000.csv", 1000000, &recordCount);
  //if (contacts == NULL || recordCount == 0)
  //{
  //  puts("Loading contact data failed. Goodbye.");
  //  exit(EXIT_FAILURE);
  //}

  //printf("There are %lu contacts in our array!\n", recordCount);
  //printf("Each contact consists of %lu bytes.\n", sizeof(contact));

  //// print info for first 10 contacts...
  //contact* tracker = contacts;
  //for (int i = 0; i < 10; i++)
  //{
  //  printContactRecord(tracker);
  //  tracker = tracker->next;
  //}

  ////(about 608 MB)
  //printf("This is %lu bytes in total.\n", sizeof(contact)*recordCount);

  //// now, write contacts out to a new binary file...
  //printf("Writing %lu records to binary file...\n", recordCount);
  //writeContacts("contacts.dat", contacts, recordCount);
  //printf("Finished writing to binary file\n");

  //// free the memory used to store the contacts
  ////free(contacts);
  //freeContacts(contacts);
  */

  //now, let's read in from binary file
  printf("Reading %lu records from binary file...\n", 1000000);
  contact * contacts2 = loadContacts("contacts.dat", 1000000, &recordCount);
  printf("Finished reading from binary file\n");

  if (contacts2 == NULL || recordCount == 0)
  {
    puts("Loading contact data failed. Goodbye.");
    exit(EXIT_FAILURE);
  }

  printf("There are %lu contacts in our array!\n", recordCount);
  printf("Each contact consists of %lu bytes.\n", sizeof(contact));
  doMergeSort(&contacts2, 0, 0, 0);

  // print info for first 10 contacts...
  contact* tracker2 = contacts2;
  int indexCounter = 0;
  for (int i = 0; i < 10; i++)
  {
    if (tracker2 != 0xCDCDCDCD) {
      printContactRecord(tracker2);
      tracker2 = tracker2->next;
    }
  }

  // free the memory used to store the contacts
  //free(contacts2);
  freeContacts(contacts2);
  printf("Completed Operations.\n");
  getch();
  return EXIT_SUCCESS;
}

/* sends contact information to consol */
void printContactRecord(contact *record)
{
  // SUPPLY YOUR CODE FOR THIS FUNCTION
  if (record != 0xCDCDCDCD) {
    printf("%s\n", record->firstName);
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
contact * loadContactCsv(char* sourceFile, unsigned long *recordsRead)
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

      (recordsRead)++; // increment list count

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

/*writes contact records to binary file*/
int writeContacts(char* fileName, contact *head)
{
  FILE *fp = fopen(fileName, "wb");

  while (head != NULL)
  {
    fwrite(head, sizeof(contact), 1, fp);
    head = head->next;
  }

  fflush(fp);
  fclose(fp);

  return 0;
}


/*loads contact records from binary file*/
contact * loadContacts(char* sourceFile, long maxRecords, unsigned long *recordsRead)
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

  fclose(fps);
  return head;
}

// Frees memory for linked list of contacts
void freeContacts(contact *head)
{
  contact *tmp;
  while (head != NULL)
  {
    tmp = head;
    head = head->next;
    free(tmp);
  }
}

//Merge Sort Functions
void doMergeSort(contact **head, int sortBy, int sortOrder,int iter) {
  printf(" ----- Iteration: %d, Head Ptr: %p, Head Address: %p, Next: %p ----- \n", iter+1, head, (*head), (*head)->next);
  //No processing for empty/single node lists.
  if (head == NULL
    || *head == NULL
    || (*head)->next == NULL) {
    printf("Terminating Early in Merge Sort.");
    return;
  }

  /*contact** temp = head;
  for (int i = 0; i < 3; i++){
    printf("%s\n",(*temp)->firstName);
    temp = &((*temp)->next);
  }*/

  contact *first, *second;
  splitList(*head, &first, &second);
  printf("First: %p, Second: %p\n", &first, &second);
  printf("-------------------------------\n");
  //doMergeSort(&first, sortBy, sortOrder, iter+1);
  //doMergeSort(&second, sortBy, sortOrder, iter+1);

  if (sortBy == 0) {
    //*head = mergeListsByLastName(first, second, sortOrder);
  }
  else {
    //*head = mergeListsByFirstName(first, second, sortOrder);
  }
  getch();
}

void splitList(contact * head, contact ** h1, contact ** h2) {
  int segmentLength = 0;
  *h1 = head;
  contact** track = head;
  /*while (&((*track)) != NULL) {
    segmentLength++;
    printf("%p, %p\n", track, (*track));
    track = (&((*track)->next));
  }
*/
  *h1 = head;

  int cnt = 0;
  while (head->next != NULL) {
    head = head->next;
    cnt ++;
    if (cnt > 1000000){
    printf("%d\n", cnt);
    }
  }

  printf("Length: %d\n", segmentLength);
  for (int i = 0; i < segmentLength; i++) {

  }

  printf("%s First\n", (*h1)->firstName);
  printf("%s Second\n", (*h1)->firstName);
}


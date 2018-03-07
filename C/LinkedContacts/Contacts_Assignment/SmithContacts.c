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
  struct contact * next;
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
} contact; //608 bytes

/* function prototypes */
unsigned int fGetLine(FILE *fps, char *line, int max_len);
void setString(char *dest, char *value);
contact * loadContactCsv(char* sourceFile, long maxRecords, unsigned long *recordsRead);
int parseContact(char * line, contact * record);
void printContactRecord(contact *record);
int writeToBinary(char* fileName, contact *contacts, size_t numRecords);
contact * loadFromBinary(char* sourceFile, long maxRecords, unsigned long *recordsRead);

/* import contacts from large text file,
* and write into a new binary file */
int main(void)
{
  unsigned long recordCount = 0;

  printf("Loading contacts from csv.  Please wait...\n");

  // load contacts from text-based csv file.
  contact * contacts = loadContactCsv("us-1000000.csv", 1000000, &recordCount);

  if (contacts == NULL || recordCount == 0)
  {
    puts("Loading contact data failed. Goodbye.");
    exit(EXIT_FAILURE);
  }

  printf("There are %lu contacts in our array!\n", recordCount);
  printf("Each contact consists of %lu bytes.\n", sizeof(contact));

  // print info for first 10 contacts...
  contact* tracker = contacts;
  for (int i = 0; i < 10; i++)
  {
    printContactRecord(tracker);
    tracker = tracker->next;
  }

  //(about 608 MB)
  printf("This is %lu bytes in total.\n", sizeof(contact)*recordCount);

  // now, write contacts out to a new binary file...
  printf("Writing %lu records to binary file...\n", recordCount);
  writeToBinary("contacts.dat", contacts, recordCount);
  printf("Finished writing to binary file\n");

  // free the memory used to store the contacts
  free(contacts);

  //now, let's read in from binary file
  printf("Reading %lu records from binary file...\n", recordCount);
  contact * contacts2 = loadFromBinary("contacts.dat", 1000000, &recordCount);
  printf("Finished reading from binary file\n");

  if (contacts2 == NULL || recordCount == 0)
  {
    puts("Loading contact data failed. Goodbye.");
    exit(EXIT_FAILURE);
  }

  printf("There are %lu contacts in our array!\n", recordCount);
  printf("Each contact consists of %lu bytes.\n", sizeof(contact));

  // print info for first 10 contacts...
  contact* tracker2 = contacts2;
  printf("%p\n%s\n", contacts2->next, ((contact*)tracker2->next)->firstName);
  int indexCounter = 0;
  for (int i = 0; i < 10; i++)
  {
    if (tracker2 != 0xCDCDCDCD) {
      printContactRecord(tracker2);
      tracker2 = tracker2->next;
    }
  }

  // free the memory used to store the contacts
  free(contacts2);
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
    fwrite(tracker->firstName, sizeof(contact), 1, ptr);
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

  // SUPPLY YOUR CODE FOR THIS FUNCTION

  // -- sourceFile: holds the name of the binary file you must read from.

  contact *  ptContacts;
  FILE *ptr;

  // -- maxRecords: tells you the max number of records to read from the file.
  //                You will use this number in your calculation for malloc...
  //                (do not read more than that)

  // -- *recordsRead: When your program is finished reading, you will indicate how many
  //                  actual records you read from the file so the called knows how many records
  //                  were read.

  // (1) open the file for binary read
  ptr = fopen(sourceFile, "rb");

  // (2) check that the pointer is valid (!=NULL)
  // (3) If not valid, do this:
  if (!ptr)
  {
    perror(sourceFile);
    return NULL;
  }
  // (4) If valid file pointer, we must calculate how many records are in the file
  //     (a) Seek to the end of the file (move to the end)
  fseek(ptr, 0L, SEEK_END);
  //     (b)  This will tell you how many contacts are in the file:
  (*recordsRead) = ftell(ptr) / sizeof(contact);
  //     (c) move back to the beginning of the file (you should know how to do this)
  fseek(ptr, 0L, 0);

  // (5) Do not read more records than you are told to:
  if (*recordsRead > maxRecords)
    *recordsRead = maxRecords;
  // (6) CRITICAL: allocate enough memory to hold the number
  //               of records you are about to read from your file.
  ptContacts = (contact *)malloc(sizeof(contact));

  // (7) Make sure your memory pointer is valid (that you were able to allocate the memory)
  if (!ptContacts)
  {
    perror("Can't allocate memory on heap.");
    fclose(ptr);
    return NULL;
  }

  // (8) read the file into memory
  char line[256];
  contact * tracker = ptContacts;
  while (fGetLine(ptr, line, sizeof(line)) > 0)
  {
    //fread(tracker, sizeof(contact), 1, ptr);
    parseContact(line, tracker);
    printf("%s\n", tracker->firstName);
    tracker->next = (contact *)malloc(sizeof(contact));
    tracker = tracker->next;
    //parseContact(line, placeHolder);
    //printf("%p: %s\n", placeHolder, placeHolder->firstName);
    //placeHolder->next = (contact *)malloc(sizeof(contact));
    //placeHolder = placeHolder->next;
  }
  // (9) close the file.
  fclose(ptr);

  // (10) return memory pointer to contacts.
  return ptContacts;
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
contact * loadContactCsv(char* sourceFile, long maxRecords, unsigned long *recordsRead)
{
  //pointer that holds access to memory we allocate to hold contacts
  contact * contacts;

  // allocate memory to hold maximum number of contact records
  contacts = (contact *)malloc(sizeof(contact));

  if (contacts == NULL)
    // we could not allocate enough memory to hold our contact data
    return NULL;

  // open csv file
  FILE *fps = fopen(sourceFile, "r");

  unsigned long mi = 0; // index into our allocated contact memory

  if (fps != NULL) // make sure we have a valid file pointer
  {
    char line[256]; // we can read a line with max 256 characters (including null terminator)

                    // Read each contact from the text file one line at a time.
                    // Note: 1 "record" = 1 line = 1 contact

    contact * placeHolder = contacts;
    while (fGetLine(fps, line, sizeof(line)) > 0 && mi < maxRecords)
    {
      // place values we read from the file into a contact record
      parseContact(line, placeHolder);
      //printf("%p: %s\n", placeHolder, placeHolder->firstName);
      placeHolder->next = (contact *)malloc(sizeof(contact));
      placeHolder = placeHolder->next;
      mi++; // increment the array index...
    }

    // let caller know how many actual records were read...
    (*recordsRead) = mi;

    // close the file
    fclose(fps);

    // Return pointer to memory (the heap) where contacts are stored
    //Returns Head
    return contacts;
  }
  else
  {
    // there was some sort of error
    perror(sourceFile);

    //free memory here
    if (contacts != NULL)
      free(contacts);

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
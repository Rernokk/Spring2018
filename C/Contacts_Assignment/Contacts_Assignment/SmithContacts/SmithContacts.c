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
#include <time.h>
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
  for (int i = 1; i <= 10; i++)
  {
    printContactRecord(&contacts[i]);
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
  for (int i = 0; i < 10; i++)
  {
    printContactRecord(&contacts2[i]);
  }
  // free the memory used to store the contacts
  free(contacts2);
  printf("\nPress any key to exit...\n");
  getch();
  return EXIT_SUCCESS;
}

/*writes contact records to binary file*/
int writeToBinary(char* fileName, contact *contacts, size_t numRecords)
{
  // SUPPLY YOUR CODE FOR THIS FUNCTION

  // -- fileName: holds the name of the file you must create
  //   			(this means that you will open a new file that will
  //    			overwrite the file if it already exists)

  // -- contacts: is a pointer to memory where all the contacts are currently storedRernokk

  // -- numRecords: holds the value telling you how many records/structs in memory
  //               (chunks) to write into the file

  //Function should return 1, if it is successful

  //(1) You need to open the file for write as binary (overwriting any existing file of same name)
  //(2) You then must write the data from memory location "contacts" into the file
  //    Note: numRecords tells you how many "Chunks" to write
  //(3) Close the file (don't forget to flush first)
  //(4) Return 1 to signify success
  FILE* fp = fopen(fileName, "wb+");
  //for (int i = 1; i <= numRecords; i++) {
    //first_name, last_name, company_name, address, city, county, state, zip, phone1, phone2, email, web
    //fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\r\n", (&contacts[i])->firstName, (&contacts[i])->lastName, (&contacts[i])->company, (&contacts[i])->street1, (&contacts[i])->city, (&contacts[i])->county, (&contacts[i])->state, (&contacts[i])->zip, (&contacts[i])->phone1, (&contacts[i])->phone2, (&contacts[i])->email, (&contacts[i])->web);
  //}
  fwrite(contacts, sizeof(contacts), numRecords, fp);
  fflush(fp);
  fclose(fp);
  return 1;
}

/*loads contact records from binary file*/
contact * loadFromBinary(char* sourceFile, long maxRecords, unsigned long *recordsRead)
{

  // SUPPLY YOUR CODE FOR THIS FUNCTION

  // -- sourceFile: holds the name of the binary file you must read from.

  // -- maxRecords: tells you the max number of records to read from the file.
  //                You will use this number in your calculation for malloc...
  //                (do not read more than that)

  // -- *recordsRead: When your program is finished reading, you will indicate how many
  //                  actual records you read from the file so the called knows how many records
  //                  were read.

  // (1) open the file for binary read
  // (2) check that the pointer is valid (!=NULL)
  // (3) If not valid, do this:
  //  		perror(sourceFile);
  //      return NULL;
  // (4) If valid file pointer, we must calculate how many records are in the file
  //     (a) Seek to the end of the file (move to the end)
  //     (b)  This will tell you how many contacts are in the file:
  //			 (*recordsRead) = ftell(fps)/sizeof(contact);
  //     (c) move back to the beginning of the file (you should know how to do this)
  // (5) Do not read more records than you are told to:
  //		If (*recordsRead)>maxRecords, then set (*recordsRead = maxRecords)
  // (6) CRITICAL: allocate enough memory to hold the number
  //               of records you are about to read from your file.
  // (7) Make sure your memory pointer is valid (that you were able to allocate the memory)
  //     If pointer is NULL, do this:
  //
  //		perror("Can't allocate memory on heap.");
  //		return NULL; //return null to indicate that no records were read
  // (8) read the file into memory
  // (9) close the file.
  // (10) return memory pointer to contacts.
  FILE* fp = fopen(sourceFile, "rb");
  rewind(fp);
  if (fp == NULL) {
    perror(sourceFile);
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  rewind(fp);
  //(*recordsRead) = ftell(fp)/sizeof(contact);
  if ((*recordsRead) > maxRecords) {
    (*recordsRead) = maxRecords;
  }
  int byteSize = (*recordsRead) * sizeof(contact);
  contact* storagePointer = (contact*) malloc(byteSize);
  if (storagePointer == NULL) {
    perror("Can't allocate memory on heap.");
    return NULL;
  }

  if (fp) {
    char c[256];
    unsigned long i = 0;
    while (fGetLine(fp, c, sizeof(c)) > 0 && i <= *recordsRead-1) {
      fseek(fp, 1, SEEK_CUR);
      parseContact(c, &storagePointer[i]);
      i+=1;
    }
  }

  fclose(fp);
  // We are returning NULL for now (because there are no records being read).
  // BUT, when your code is in place, you will return
  // the memory pointer you got back from malloc()
  return storagePointer; // zero records read
}


/* sends contact information to console */
void printContactRecord(contact *record)
{
  return;
  // SUPPLY YOUR CODE FOR THIS FUNCTION
  printf("\n*** Contact Information ***\n");
  printf("First Name: %s\n", record->firstName);
  printf("Last Name: %s\n", record->lastName);
  printf("Company: %s\n", record->company);
  printf("Address: %s\n", record->street1);
  printf("City: %s\n", record->city);
  printf("State: %s\n", record->state);
  printf("Zipcode: %s\n", record->zip);
  printf("Email: %s\n", record->email);
  printf("Web: %s\n", record->web);
  printf("Phone: %s\n", record->phone1);
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
  contacts = (contact *)malloc(maxRecords * sizeof(contact));

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
    while (fGetLine(fps, line, sizeof(line)) > 0 && mi < maxRecords)
    {
      // place values we read from the file into a contact record
      parseContact(line, &contacts[mi]);
      mi++; // increment the array index...
    }

    // let caller know how many actual records were read...
    (*recordsRead) = mi;

    // close the file
    fclose(fps);

    // Return pointer to memory (the heap) where contacts are stored
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

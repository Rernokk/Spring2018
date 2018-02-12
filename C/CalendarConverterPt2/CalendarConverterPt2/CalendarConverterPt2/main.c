/*
  Shannon Smith
  Using the provided solution from Project 1
*/

#include <stdio.h>
#include <math.h>

int getIntFromUser(char*, int);
double getDoubleFromUser(char*);
void pause_p(void);

void setJulianDay(struct date *d);
double getFullJulianDay(const struct date *d);
struct date addDays(double nDays, const struct date *d);
struct date addMonths(double nMonths, const struct date *d);
struct date addYears(double nYears, const struct date *d);
struct date addHours(double nHours, const struct date *d);
struct date addMinutes(double nMinutes, const struct date *d);
struct date addSeconds(double nSeconds, const struct date *d);
struct date getDateFromJulian(double jd);

//double getJulianDay(int M, int D, int Y);
//float getDayPart(int hour, int minutes, int seconds);

/*
  Struct Data
*/
struct date {
  int month, day, year, hr, min, sec;
  double jd;
};

/*
Functions for calculating dates based on Julian day conversions
*/

int main()
{
  int d, m, y, h, n, s;
  double jd;
  ////get date
  //y = getIntFromUser("Enter year: ", -10000);
  //m = getIntFromUser("Enter month: ", 0);
  //d = getIntFromUser("Enter day: ", 0);

  ////get time of date (universal time)
  //h = getIntFromUser("Enter hour of day (0-23): ", 0);
  //n = getIntFromUser("Enter minutes (0-59): ", 0);
  //s = getIntFromUser("Enter seconds (0-59): ", 0);

  //jd = getJulianDay(m, d, y);

  //printf("The Julian Day is %f \n", jd + getDayPart(h, n, s));
  //printf("The day part is %f \n", getJulianDay(m, d, y));
  //printf("The time part is %f \n", getDayPart(h, n, s));

  //printf("\n");

  //getDateFromJulian(jd, &m, &d, &y);

  //printf("Converted back: %d, %d, %d \n", m, d, y);

  jd = getDoubleFromUser("Enter a valid Julian Day:");

  if (jd != -999.0)
  {
    //  Now, convert this jd into calendar date
    //  (we ignore time of day for now)
    struct date val = getDateFromJulian(jd);
    //val = addDays(9883, &val);
    val = addMonths(1, &val);
    printf("Month, day, year is: %d, %d, %d \n", val.month, val.day, val.year);
  }
  
  getch();
}

// Displays message to user and gets int as input
int getIntFromUser(char* msg, int min)
{
  int input = min - 1;
  char term; // used to check for valid numeric input

  while (input<min)
  {

    // Get user’s input.
    printf("%s\n", msg);

    // check for valid integer (this will be explained later)
    if (scanf("%d%c", &input, &term) != 2 || term != '\n')
    {
      printf("That's not an integer!\n");
      break;
    }

    if (input<min)
      printf("Please enter a value of %d or greater!\n", min);

  }
  return input;
}

double getDoubleFromUser(char* msg)
{
  double input = 0;
  char term;

  // Get user’s input.
  printf("%s\n", msg);

  // check for valid number (this will be explained later)
  if (scanf("%lf%c", &input, &term) != 2)
  {
    if (term >= 0x41 && term <= 0x7A)
    {
      printf("That's not a valid number!\n");
      return -999.0;
    }
  }
  return input;
}

/* old GetJulianDay code
double getJulianDay(int M, int D, int Y)
{
  int B, A;
  double JD;

  if (M <= 2)
  {
    Y--;
    M += 12;
  }

  A = floor(Y / 100.0);
  B = 2 - A + floor(A / 4.0);

  //Now, get JD
  JD = floor((365.25*(Y + 4716.0)))
    + floor((30.6001*(M + 1))) + D + B - 1524.5;

  return JD;
}
*/

/* old GetDayPart Code
float getDayPart(int hour, int minutes, int seconds)
{
  // valid values:
  //  hour: 0-23, minutes: 0-59, seconds: 0-59
  return (((hour*60.0f)*60.0f) + (minutes*60.0f) + seconds) / 86400.0f;

}
*/

struct date getDateFromJulian(double jd)
{
  struct date newDate;
  int a, b, c, d, e, alpha;
  double z, f;

  jd += .5;

  // For now, we are not including the time of day in our calculations.
  // If we were, d would be a pointer to a float
  f = modf(jd, &z);

  if (z<2299161)
    a = z;
  else
  {
    alpha = floor((z - 1867216.25) / 36524.25);
    a = z + 1 + alpha - floor(alpha / 4);
  }

  //perform setup calculations
  b = a + 1524;
  c = floor((b - 122.1) / 365.25);
  d = floor(365.25*c);
  e = floor((b - d) / 30.6001);

  // Now, let's find the calendar components...
  // (day)
  newDate.day = b - d - floor(30.6001 * e) + f;

  // (month)
  if (e<14)
    newDate.month = e - 1;
  else
    newDate.month = e - 13;

  // (year)
  if (newDate.month >2)
    newDate.year = c - 4716;
  else
    newDate.year = c - 4715;
  
  newDate.jd = jd;
  // time of day
  if (f>0.0)
  {
    int seconds = f * 86400.0;
    newDate.hr = (int)(seconds / 3600);
    newDate.min = (int)((seconds % 3600) / 60);
    newDate.sec = modf((seconds % 3600) / 60.0, &z) * 60;
  }
  else
  {
    newDate.hr = 0;
    newDate.min = 0;
    newDate.sec = 0;
  }

  return newDate;
}

void pause_p(void)
{
  printf("\n");
  printf("Press <ENTER> to continue...");
  getchar();
  return;
}

//Project 2 Methods
double getFullJulianDay(const struct date *d) {
  return d->jd;
}

void setJulianDay(struct date *d) {
  d->jd = getFullJulianDay(d);
}

struct date addDays(double nDays, const struct date *d) {
  struct date newDate = *d;
  newDate.jd += nDays - 1;
  newDate = getDateFromJulian(newDate.jd);
  return newDate;
}

struct date addMonths(double nMonths, const struct date *d) {
  struct date newDate = *d;
  newDate.jd += nMonths * 30.44;
  newDate = getDateFromJulian(newDate.jd);
  return newDate;
}

struct date addYears(double nYears, const struct date *d) {

}

struct date addHours(double nHours, const struct date *d) {

}

struct date addMinutes(double nMinutes, const struct date *d) {

}

struct date addSeconds(double nSeconds, const struct date *d) {

}
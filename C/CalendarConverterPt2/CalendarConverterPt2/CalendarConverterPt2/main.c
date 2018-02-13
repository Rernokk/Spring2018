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
struct date createDate (int month, int day, int year, int hr, int minute, int second, double JD);
struct date addDays(double nDays, const struct date *d);
struct date addWeeks(double nWeeks, const struct date *d);
struct date addMonths(double nMonths, const struct date *d);
struct date addYears(double nYears, const struct date *d);
struct date addHours(double nHours, const struct date *d);
struct date addMinutes(double nMinutes, const struct date *d);
struct date addSeconds(double nSeconds, const struct date *d);
struct date getDateFromJulian(double jd);

double getSecondsDiff(const struct date *d1, const struct date *d2);
double getHourDiff(const struct date *d1, const struct date *d2);
double getDayDiff(const struct date *d1, const struct date *d2);
double getWeekDiff(const struct date *d1, const struct date *d2);
double getMonthDiff(const struct date *d1, const struct date *d2);
double getYearDiff(const struct date *d1, const struct date *d2);

//double getJulianDay(int M, int D, int Y);
//float getDayPart(int hour, int minutes, int seconds);

/*
  Struct Data
*/
struct date {
  int month, day, year, hr, min, sec;
  double jd;
};

struct date createDate(int month, int day, int year, int hr, int minute, int second, double JD) {
  struct date d;
  d.year = year;
  d.month = month;
  d.day = day;
  d.hr = hr;
  d.min = minute;
  d.sec = second;
  setJulianDay(&d);
  return d;
}

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

  //jd = getDoubleFromUser("Enter a valid Julian Day:");
  jd = 0;
  if (jd != -999.0)
  {
    printf("Problem 3-%d:\n", 1);
    struct date pThreeOne = createDate(2, 2, 2017, 0, 0, 0, 0);
    struct date pThreeOnePtTwo = addSeconds(1000000000, &pThreeOne);
    printf("Start Date: %d/%d/%d at %d:%d:%d, End Date: %d/%d/%d at %d:%d:%d, after adding 1,000,000,000 seconds.\n", pThreeOne.month, pThreeOne.day, pThreeOne.year, pThreeOne.hr, pThreeOne.min, pThreeOne.sec, pThreeOnePtTwo.month, pThreeOnePtTwo.day, pThreeOnePtTwo.year, pThreeOnePtTwo.hr, pThreeOnePtTwo.min, pThreeOnePtTwo.sec);

    printf("\nProblem 3-%d:\n", 2);
    struct date pThreeTwo = createDate(3, 3, 1966, 0, 0, 0, 0);
    struct date pThreeTwoPtTwo = addDays(14654, &pThreeTwo);
    printf("Start Date: %d/%d/%d at %d:%d:%d, End Date: %d/%d/%d at %d:%d:%d, after adding 14,654 days.\n", pThreeTwo.month, pThreeTwo.day, pThreeTwo.year, pThreeTwo.hr, pThreeTwo.min, pThreeTwo.sec, pThreeTwoPtTwo.month, pThreeTwoPtTwo.day, pThreeTwoPtTwo.year, pThreeTwoPtTwo.hr, pThreeTwoPtTwo.min, pThreeTwoPtTwo.sec);

    printf("\nProblem 3-%d:\n", 3);
    struct date pTThr = createDate(4, 18, 1973, 0, 0, 0, 0);
    struct date pTThrT = addMonths(68, &pTThr);
    printf("Start Date: %d/%d/%d at %d:%d:%d, End Date: %d/%d/%d at %d:%d:%d, after adding 68 months.\n", pTThr.month, pTThr.day, pTThr.year, pTThr.hr, pTThr.min, pTThr.sec, pTThrT.month, pTThrT.day, pTThrT.year, pTThrT.hr, pTThrT.min, pTThrT.sec);

    printf("\nProblem 3-%d:\n", 4);
    struct date pTF = createDate(2,9,17,12,0,0,0);
    struct date pTFT = createDate(11,25,2034, 20, 0, 0, 0);
    printf("We will have to wait %f seconds for the next supermoon on 11/25/2034.\n", getSecondsDiff(&pTF, &pTFT));

    printf("\nProblem 3-%d:\n", 5);
    struct date pTFi = createDate(2,9,2017,16,51,0,0);
    struct date pTFiT = addMinutes(893, &pTFi);
    printf("Start Date: %d/%d/%d at %d:%d:%d, End Date: %d/%d/%d, at %d:%d:%d, after adding 893 minutes.\n", pTFi.month, pTFi.day, pTFi.year, pTFi.hr, pTFi.min, pTFi.sec, pTFiT.month, pTFiT.day, pTFiT.year, pTFiT.hr, pTFiT.min, pTFiT.sec);

    printf("\nProblem 3-%d:\n", 6);
    struct date pTSix = createDate(2,9,2017, 5, 13, 0, 0);
    struct date pTSixT = addYears(-657.76, &pTSix);
    printf("Start Date: %d/%d/%d at %d:%d:%d, End Date: %d/%d/%d, at %d:%d:%d, after adding -675.76 years.\n", pTSix.month, pTSix.day, pTSix.year, pTSix.hr, pTSix.min, pTSix.sec, pTSixT.month, pTSixT.day, pTSixT.year, pTSixT.hr, pTSixT.min, pTSixT.sec);

    printf("\nProblem 3-%d:\n", 7);
    struct date pTSev = createDate(4,18,1973, 0, 0, 0, 0);
    struct date pTSevT = addMonths(-68, &pTSev);
    printf("Start Date: %d/%d/%d at %d:%d:%d, End Date: %d/%d/%d at %d:%d:%d, after subtracting 68 months.\n", pTSev.month, pTSev.day, pTSev.year, pTSev.hr, pTSev.min, pTSev.sec, pTSevT.month, pTSevT.day, pTSevT.year, pTSevT.hr, pTSevT.min, pTSevT.sec);

    printf("\nProblem 3-%d:\n", 8);
    struct date pTEight = createDate(8,18,1967,4,15,30,0);
    struct date pTEightT = addSeconds(57865, &pTEight);
    printf("Start Date: %d/%d/%d at %d:%d:%d, End Date: %d/%d/%d, at %d:%d:%d, after adding 57,865 seconds.\n", pTEight.month, pTEight.day, pTEight.year, pTEight.hr, pTEight.min, pTEight.sec, pTEightT.month, pTEightT.day, pTEightT.year, pTEightT.hr, pTEightT.min, pTEightT.sec);

    printf("\nProblem 3-%d:\n", 9);
    struct date pTNine = createDate(2,23,2017,0,0,0,0);
    struct date pTNineT = createDate(6,13,2018,0,0,0,0);
    printf("The number of seconds between %d/%d/%d and %d/%d/%d is %f.\n", pTNine.month, pTNine.day, pTNine.year, pTNineT.month, pTNineT.day, pTNineT.year, getSecondsDiff(&pTNine, &pTNineT));

    printf("\nProblem 3-%d:\n", 10);
    struct date pTTen = createDate(2, 23, 2017, 0, 0, 0, 0);
    struct date pTTenT = createDate(6, 13, 2018, 0, 0, 0, 0);
    printf("The number of days between %d/%d/%d and %d/%d/%d is %f.\n", pTTen.month, pTTen.day, pTTen.year, pTTenT.month, pTTenT.day, pTTenT.year, getDayDiff(&pTTen, &pTTenT));
    //val = addMonths(1, &val);
    //val = addSeconds(0, &val);
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

float getDayPart(int hour, int minutes, int seconds)
{
  // valid values:
  //  hour: 0-23, minutes: 0-59, seconds: 0-59
  return (((hour*60.0f)*60.0f) + (minutes*60.0f) + seconds) / 86400.0f;

}

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
  double val = getJulianDay(d->month, d->day, d->year) + getDayPart(d->hr, d->min, d->sec);
  return val;
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

struct date addWeeks(double nWeeks, const struct date *d) {
  struct date newDate = *d;
  newDate.jd += nWeeks * 7;
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
  struct date newDate = *d;
  newDate.jd += nYears * 365;
  newDate = getDateFromJulian(newDate.jd);
  return newDate;
}

struct date addHours(double nHours, const struct date *d) {
  struct date newDate = *d;
  newDate.jd += nHours / 24.0;
  newDate = getDateFromJulian(newDate.jd);
  return newDate;
}

struct date addMinutes(double nMinutes, const struct date *d) {
  struct date newDate = *d;
  newDate.jd += nMinutes / 1440;

  newDate = getDateFromJulian(newDate.jd);
  return newDate;

}

struct date addSeconds(double nSeconds, const struct date *d) {
  struct date newDate = *d;
  newDate.jd += nSeconds / 86400;
  newDate = getDateFromJulian(newDate.jd);
  return newDate;

}

double getDayDiff(const struct date *d1, const struct date *d2) {
  return (d2->jd - d1->jd);
}

double getYearDiff(const struct date *d1, const struct date *d2){
  double val = getDayDiff(d1, d2);
  val = val/365.25;
  return val;
}

double getWeekDiff(const struct date *d1, const struct date *d2) {
  double val = getDayDiff(d1, d2);
  val = val / 52.0;
  return val;
}

double getMonthDiff(const struct date *d1, const struct date *d2) {
  double val = getDayDiff(d1, d2);
  val = val / 30.44;
  return val;
}

double getHourDiff(const struct date *d1, const struct date *d2) {
  double val = getDayDiff(d1, d2);
  val = val * 24;
  return val;
}

double getSecondsDiff(const struct date *d1, const struct date *d2) {
  double val = getHourDiff(d1, d2);
  val = val * 3600;
  return val;
}
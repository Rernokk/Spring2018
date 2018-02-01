/******************************************************************************

Online C Compiler.
Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <math.h>
//double getDoubleFromUser(char*);

double GetJulianDay(int, int, int);
float GetDayPart(int, int, int);
void GetDateFromJulian(double, int*, int*, int*);

float Problem1();
void Problem2(float julian, int deltaDays);

int
main()
{
  float jul = Problem1();
  printf("How many days would you like to move?\n");
  int dayCount;
  scanf("%d", &dayCount);
  Problem2(jul, dayCount);
  
  printf("\n");
  printf("Press <ENTER> to continue...\n");
  scanf("%d", &dayCount);
}

//double
//getDoubleFromUser(char* msg) {
//  printf(msg);
//  char input, term;
//  if (scanf("%lf%c"), &input, &term) != 2){
//    if (term >= 0x41 && term <= 0x7A) {
//      printf("That's not a valid number!\n");
//      return -999.0;
//    }
//  }
//  return input;
//}

double
GetJulianDay(int Y, int M, int D)
{
  int locY = Y, locM = M, locD = D;
  if (locM <= 2) {
    locY--;
    locM+=12;
  } else if (locM == 1 || locM == 2) {
    locY = locY - 1;
    locM = locM + 12;
  }

  int A = floor(locY / 100);
  int B = 2 - A + floor(A / 4);
  return floor((365.25 * (locY + 4716.0)) + floor((30.6001 * (locM + 1)))) + locD +
    B - 1524.5;
}

float
GetDayPart(int H, int M, int S)
{
  float hr = H * 60.0f * 60.0f;
  float min = M * 60.0f;
  float s = S;
  return (hr + min + s) / 86400.0f;
}

float
Problem1()
{
  int y, m, d, h, n, s;
  printf("Enter the year: ");
  scanf("%d", &y);
  printf("Enter the month: ");
  scanf("%d", &m);
  printf("Enter the day: ");
  scanf("%d", &d);
  printf("Enter the hour: ");
  scanf("%d", &h);
  printf("Enter the minute: ");
  scanf("%d", &n);
  printf("Enter the second: ");
  scanf("%d", &s);
  double day = GetJulianDay(y, m, d);
  double dayPart = GetDayPart(h, n, s);
  printf("The Julian Day is: %f\nThe day part is %f\nThe time part is %f\n",
    day + dayPart, day, dayPart);
  return day + dayPart;
}

void
GetDateFromJulian(double jd, int *month, int *day, int *year)
{
  int A, B, C, D, E, alpha;
  double Z, F;
  jd += 0.5;
  F = modf(jd, &Z);
  if (Z < 2299161) {
    A = Z;
  }
  else {
    alpha = (int)((Z - 1867216.25) / 36524.25);
    A = Z + 1 + alpha - (int)(alpha / 4);
  }

  B = A + 1524;
  C = (int)((B - 122.1) / 365.25);
  D = (int)(365.25 * C);
  E = (int)((B - D) / 30.6001);

  (*day) = B - D - ((int)(30.6001 * E)) + F;
  if (E < 14) {
    (*month) = E - 1;
  }
  else if (E == 15 || E == 14) {
    (*month) = E - 13;
  }
  if (*month > 2) {
    (*year) = C - 4716;
  }
  else if (*month == 1 || *month == 2) {
    (*year) = C - 4715;
  }
}

void
Problem2(float jul, int deltaDays)
{
  /*float jd = -999.0;
  int m, d, y;
  //getDoubleFromUser("Enter a valid Julian Day:");
  printf("Enter a valid Julian Day:\n");
  scanf("%f", &jd);*/
  float jd = jul + deltaDays;
  int m, d, y;
  if (jd != -999.0) {
    GetDateFromJulian(jd, &m, &d, &y);
    printf("Month, day, year is: %d, %d, %d\n", m, d, y);
  }
  return 0;
}

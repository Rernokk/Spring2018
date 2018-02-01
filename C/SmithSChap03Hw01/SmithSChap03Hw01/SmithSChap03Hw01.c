#include <stdio.h>

void Problem3();
void Problem4();
void Problem6();
void Problem7();
void Problem8();

void main() {
  printf("--- Problem 3 ---\n");
  Problem3();
  printf("\n--- Problem 4 ---\n");
  Problem4();
  printf("\n--- Problem 6 ---\n");
  Problem6();
  printf("\n--- Problem 7 ---\n");
  Problem7();
  printf("\n--- Problem 8 ---\n");
  Problem8();
  printf("\nEnd of Program.\n");
  int pause = 0;
  scanf("%d", &pause);
}

void Problem3() {
  printf("How many years old are you?\n");
  int yrs;
  scanf("%d", &yrs);
  printf("So you are a %d-year-old, and you are %d days old!\n", yrs, yrs * 365);
}

void Jolly();
void Deny();

void Problem4() {
  for (int i = 0; i < 3; i++) {
    Jolly();
  }
  Deny();
}

void Jolly() {
   printf("For he's a jolly good fellow!\n");
}

void Deny() {
  printf("Which nobody can deny!\n");
}

void Problem6() {
  int toes = 10;
  printf("Toes: %d, Toes Doubled: %d, Toes Squared: %d\n", toes, toes * 2, toes * toes);
}

void Problem7() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3 - i; j++) {
      printf("Smile!");
    }
    printf("\n");
  }
}

void One_Three();
void Two();

void Problem8() {
  printf("Starting Now:\n");
  One_Three();
  printf("Done!\n");
}

void One_Three() {
  printf("One\n");
  Two();
  printf("Three\n");
}

void Two() {
  printf("Two\n");
}
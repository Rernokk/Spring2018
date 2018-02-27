#include <stdio.h>
#include <stdlib.h>

void drawMap();

void main() {
  drawMap();
  //Game Loop
  char in;
  while ((in = getch()) != 'q')
  {
    system("cls");
    drawMap();
    printf("%c\n", in);
  }
  return;
}

void drawMap() {
  //Map Setup.
  printf("X\n");
}
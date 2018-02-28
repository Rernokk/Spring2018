#include <stdio.h>
#include <stdlib.h>
#include <windows.graphics.h>

void drawMap();
char* generateRoom(int width, int height);
char* catStrings (char* str1, char* str2);

void main() {
  int gameWidth = 100, gameHeight = 50;
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
  //printf("XXXXXXXXXX\n");
  printf(generateRoom(119,30));
}

char* generateRoom(int height, int width){
  char* room = "\0";
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      if ((i == 0 || i == width-1) || (j == 0 || j == height-1)){
        room = catStrings(room, "X");
      }
      else {
        room = catStrings(room, " ");
      }
    }
    room = catStrings(room, "\n");
  }

  return room;
}

char* catStrings(char* str1, char* str2) {
  char* temp;
  if ((temp = malloc(strlen(str1) + strlen(str2) + 1)) != NULL) {
    temp[0] = '\0';
    strcat(temp, str1);
    strcat(temp, str2);
  }
  return temp;
}
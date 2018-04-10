#include <curses.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//Object declarations and definitions.
typedef struct playerObject {
  int x;
  int y;
} player;

typedef struct drawObject {
  char name[32];
  int width;
  int height;
  char ** data;
} image;

//Accessible Methods For Various Purposes.
void drawBox(int tlX, int tlY, int height, int width, int hollow) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (hollow != 1) {
        mvprintw(tlX + i, tlY + j, "X");
      }
      else {
        if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
          mvprintw(tlX + i, tlY + j, "X");
        }
        else {
          mvprintw(tlX + i, tlY + j, " ");
        }
      }
    }
  }
}
void drawChar(char val, int posX, int posY) {
  switch (val) {
  case ('A'):
    mvprintw(posY, posX + 1, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 1, posX + 2, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 2, posX + 2, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY + 3, posX + 2, "X");
    mvprintw(posY + 4, posX + 2, "X");
    mvprintw(posY + 4, posX, "X");
    break;
  case ('B'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX, "X");
    mvprintw(posY, posX + 1, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY + 4, posX + 1, "X");
    mvprintw(posY + 1, posX + 2, "X");
    mvprintw(posY + 3, posX + 2, "X");
    break;
  case('C'):
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY, posX + 1, "X");
    mvprintw(posY + 4, posX + 1, "X");
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 4, posX + 2, "X");
    break;
  case('D'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX, "X");

    mvprintw(posY, posX + 1, "X");
    mvprintw(posY + 4, posX + 1, "X");
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 4, posX + 2, "X");

    mvprintw(posY + 1, posX + 3, "X");
    mvprintw(posY + 2, posX + 3, "X");
    mvprintw(posY + 3, posX + 3, "X");
    break;
  case('E'):
    mvprintw(posY, posX + 1, "X");
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX + 1, "X");
    mvprintw(posY + 4, posX + 2, "X");

    //Corners
    //mvprintw(posY, posX, "X");
    //mvprintw(posY + 4, posX, "X");
    break;

  case('F'):
    mvprintw(posY, posX, "X");
    mvprintw(posY, posX + 1, "X");
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX, "X");
    break;
  case('G'):
    break;
  case('H'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 1, posX + 2, "X");
    mvprintw(posY + 2, posX + 2, "X");
    mvprintw(posY + 3, posX + 2, "X");
    mvprintw(posY + 4, posX + 2, "X");
    break;
  case('I'):
    mvprintw(posY, posX, "X");
    mvprintw(posY, posX + 1, "X");
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 1, posX + 1, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY + 3, posX + 1, "X");
    mvprintw(posY + 4, posX, "X");
    mvprintw(posY + 4, posX + 1, "X");
    mvprintw(posY + 4, posX + 2, "X");
    break;
  case('J'):
    mvprintw(posY, posX + 1, "X");
    mvprintw(posY, posX, "X");
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 1, posX + 2, "X");
    mvprintw(posY + 2, posX + 2, "X");
    mvprintw(posY + 3, posX + 2, "X");
    mvprintw(posY + 4, posX + 1, "X");
    mvprintw(posY + 4, posX, "X");
    break;
  case('K'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX, "X");

    mvprintw(posY, posX + 3, "X");
    mvprintw(posY + 1, posX + 2, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY + 3, posX + 2, "X");
    mvprintw(posY + 4, posX + 3, "X");
    break;
  case('L'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX, "X");
    mvprintw(posY + 4, posX + 1, "X");
    mvprintw(posY + 4, posX + 2, "X");
    break;
  case('M'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX, "X");

    mvprintw(posY + 1, posX + 1, "X");
    mvprintw(posY + 2, posX + 2, "X");
    mvprintw(posY + 1, posX + 3, "X");

    mvprintw(posY, posX + 4, "X");
    mvprintw(posY + 1, posX + 4, "X");
    mvprintw(posY + 2, posX + 4, "X");
    mvprintw(posY + 3, posX + 4, "X");
    mvprintw(posY + 4, posX + 4, "X");
    break;
  case('N'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX, "X");
    mvprintw(posY + 1, posX + 1, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY + 2, posX + 2, "X");
    mvprintw(posY + 3, posX + 2, "X");
    mvprintw(posY + 1, posX + 3, "X");
    mvprintw(posY + 2, posX + 3, "X");
    mvprintw(posY + 3, posX + 3, "X");
    mvprintw(posY + 4, posX + 3, "X");
    mvprintw(posY, posX + 3, "X");
    break;
  case('O'):
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY, posX + 1, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 1, posX + 3, "X");
    mvprintw(posY + 2, posX + 3, "X");
    mvprintw(posY + 3, posX + 3, "X");
    mvprintw(posY + 4, posX + 2, "X");
    mvprintw(posY + 4, posX + 1, "X");
    break;
  case('P'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX, "X");
    mvprintw(posY, posX + 1, "X");
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 1, posX + 3, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY + 2, posX + 2, "X");
    break;
  case('Q'):
    break;
  case('R'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX, "X");
    mvprintw(posY, posX + 1, "X");
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 1, posX + 3, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY + 2, posX + 2, "X");
    mvprintw(posY + 3, posX + 3, "X");
    mvprintw(posY + 4, posX + 3, "X");
    break;

  case('S'):
    mvprintw(posY, posX + 1, "X");
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY + 2, posX + 2, "X");
    mvprintw(posY + 3, posX + 2, "X");
    mvprintw(posY + 4, posX, "X");
    mvprintw(posY + 4, posX + 1, "X");

    //Corners
    //mvprintw(posY, posX, "X");
    //mvprintw(posY + 4, posX + 2, "X");
    break;

  case('T'):
    mvprintw(posY, posX, "X");
    mvprintw(posY, posX + 1, "X");
    mvprintw(posY, posX + 3, "X");
    mvprintw(posY, posX + 4, "X");
    mvprintw(posY + 1, posX + 4, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 1, posX + 2, "X");
    mvprintw(posY + 2, posX + 2, "X");
    mvprintw(posY + 3, posX + 2, "X");
    mvprintw(posY + 4, posX + 2, "X");

    break;
  case('U'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX + 1, "X");
    mvprintw(posY + 4, posX + 2, "X");
    mvprintw(posY, posX + 3, "X");
    mvprintw(posY + 1, posX + 3, "X");
    mvprintw(posY + 2, posX + 3, "X");
    mvprintw(posY + 3, posX + 3, "X");
    break;
  case('V'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY + 3, posX + 1, "X");
    mvprintw(posY + 4, posX + 2, "X");
    mvprintw(posY, posX + 4, "X");
    mvprintw(posY + 1, posX + 4, "X");
    mvprintw(posY + 2, posX + 3, "X");
    mvprintw(posY + 3, posX + 3, "X");
    break;
  case('W'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX + 1, "X");
    mvprintw(posY + 3, posX + 1, "X");
    mvprintw(posY + 2, posX + 2, "X");
    mvprintw(posY + 3, posX + 3, "X");
    mvprintw(posY, posX + 4, "X");
    mvprintw(posY + 1, posX + 4, "X");
    mvprintw(posY + 2, posX + 4, "X");
    mvprintw(posY + 3, posX + 4, "X");
    mvprintw(posY + 4, posX + 3, "X");
    break;
  case('X'):
    mvprintw(posY, posX, "X");
    mvprintw(posY + 1, posX, "X");
    mvprintw(posY + 3, posX, "X");
    mvprintw(posY + 4, posX, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 1, posX + 2, "X");
    mvprintw(posY + 3, posX + 2, "X");
    mvprintw(posY + 4, posX + 2, "X");
    break;
  case('Y'):
    mvprintw(posY, posX + 2, "X");
    mvprintw(posY + 1, posX + 2, "X");
    mvprintw(posY + 2, posX + 2, "X");
    mvprintw(posY + 3, posX + 2, "X");
    mvprintw(posY + 4, posX + 2, "X");
    mvprintw(posY + 4, posX + 1, "X");
    mvprintw(posY + 4, posX, "X");

    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 2, posX + 1, "X");
    mvprintw(posY, posX, "X");
    mvprintw(posY + 2, posX, "X");
    mvprintw(posY + 1, posX, "X");
    break;
  case('Z'):
    break;
  }
}
void drawMenu(int sel) {
  drawBox(1, 1, 98, 98, TRUE);
  drawBox(3, 3, 94, 94, TRUE);
  //drawBox(25, 25, 10, 10, TRUE);
}
void drawText(int startX, int startY, char* text, int highlighted, int* ptr) {
  if (highlighted == 1) {
    attron(COLOR_PAIR(*ptr));
    mvprintw(startY, startX, text);
    mvprintw(startY + 1, startX, "%d", (*ptr));
    attroff(COLOR_PAIR(*ptr));
    (*ptr)++;
    if (*ptr > 16) {
      *ptr = 0;
    }
  }
  else {
    mvprintw(startY, startX, text);
  }
}
void * MyMiscThread(void * vargp) {
  mvprintw(50, 50, "XXX");
  return NULL;
}
unsigned int fGetLine(FILE *fps, char *line) {
  int tempChar;
  unsigned int tempCharIdx = 0U;
  while ((tempChar = fgetc(fps)))
  {
    if (tempChar == EOF) // reached end of file?
    {
      line[tempCharIdx] = '\0';
      break;
    }
    else if (tempChar == '\n'
      || tempChar == '\r') {
      line[tempCharIdx] = '\0';
      break;
    }
    else {
      line[tempCharIdx++] = (char)tempChar;
    }
  }
  return tempCharIdx;
}
void LoadAsciiFile(image * fileName) {
  /*FILE* io = fopen(fileName, "r");
  fseek(io, 0, SEEK_END);
  rewind(io);
  if (io) {
    char image[1024];
    int drawStart = 0;
    while (fGetLine(io, image) > 0) {
      mvprintw(drawStart, 10, image);
      drawStart++;
    }
  }
  fclose(io);*/
  printf("Loading %s from ASCII...\n", fileName->name);
  FILE* io = fopen(fileName->name, "r");

  fseek(io, 0L, SEEK_END);
  size_t size = ftell(io) + 2;
  fseek(io, 0L, SEEK_SET);

  int lineLength = 0;
  for (;;) {
    int c = fgetc(io);
    if (c == EOF || c == '\n') {
      break;
    }
    lineLength++;
  }
  //printf("Chars: %d,Chars in line: %d, Lines: %d\n", size, lineLength, size / (lineLength + 2));
  rewind(io);
  fileName->data = (char**)malloc(sizeof(char**) * (size / lineLength + 2));
  for (int i = 0; i < (size / (lineLength + 2)); i++) {
    fileName->data[i] = (char*)malloc(sizeof(char*) * (lineLength + 1));
    fGetLine(io, fileName->data[i]);
  }
  fileName->width = lineLength;
  fileName->height = size / (lineLength + 2);
  fclose(io);
  printf("Loading %s, complete.\n", fileName->name);
}
void DrawAsciiFile(image * obj, int startX, int startY) {
  for (int i = 0; i < obj->height; i++) {
    mvprintw(startY + i, startX, obj->data[i]);
  }
}
void DrawMap(WINDOW* win, image * map, int playerX, int playerY) {
  int offset = 3;
  for (int i = -50; i <= 50; i++) {
    for (int j = -50; j <= 50; j++) {
      if ((i + playerY < 0 || i + playerY >= map->height) || (j + playerX < 0 || j + playerX >= map->width)) {
        mvwprintw(win, i + 0, j + 0, " ");
      }
      else {
        char * c = map->data[i + playerY][j + playerX];
        if (i == 25 && j == 25) {
          wattron(win, COLOR_PAIR(2));
          mvwprintw(win, i, j, "X");
          wattroff(win, COLOR_PAIR(2));
        }
        else {
          mvwprintw(win, i, j, &c);
        }
      }
    }
  }
}
char FetchMapChar(image * map, int tarX, int tarY) {
  if (tarX >= 0 && tarY >= 0 && tarX < map->height && tarY < map->width) {
    return map->data[tarY][tarX];
  }
  return '@';
}
//Main Game
int main()
{
  WINDOW* mainWindow = initscr();                    /* Start curses mode */
  nodelay(mainWindow, TRUE);
  noecho();
  curs_set(0);
  resize_term(101, 101);
  if (has_colors == FALSE) {
    endwin();
    printf("No support for color");
    exit(1);
  }

  //Window Dimensions
  int mainWindowMaxX = 0, mainWindowMaxY = 0;
  getmaxyx(mainWindow, mainWindowMaxX, mainWindowMaxY);

  //Defining Color Pairs & Color Initialization
  start_color();
  init_color(COLOR_CYAN, 0, 0, 1000);
  init_pair(1, COLOR_RED, COLOR_WHITE);
  for (int i = 2; i < 16; i++) {
    init_pair(i, COLOR_YELLOW, COLOR_BLACK);
  }
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_BLACK, COLOR_WHITE);

  //Defining Subwindows
  WINDOW* mainMenuInsetWindow = subwin(mainWindow, 60, 60, 3, 3);

  //Object Initialization and variables.
  player Player = { -21, -21 };
  int currentScene = 0;
  /* Scene Definitions & Purposes
     0 - Main Menu, player interacts w/ Scene to start game via Up/Down or W/S.
     1 - World Scene, player navigates 2D realm, overall HUD.

  */

  int selectedMenu = 0;
  int temp = 0;

  //Pre-loading artwork.
  image skull0 = { "Skull0.txt" };
  image phoenix = { "Phoenix.txt" };
  image mainMap = { "mainMap.txt" };
  LoadAsciiFile(&skull0);
  LoadAsciiFile(&phoenix);
  LoadAsciiFile(&mainMap);

  //Denoting ASCII Symbols
  char wall = 'x';

  //Game Loop
  while (TRUE) {

    //Enabling Color_Pair(1) on text, drawing box outline
    /*attron(COLOR_PAIR(1));
    drawMenu(selectedMenu);
    attroff(COLOR_PAIR(1));*/


    //Menu Options
    /*switch (selectedMenu) {
    case (0):
      drawText(25, 25, "ABC", 1, &temp);
      drawText(25, 30, "DEF", 0, &temp);
      drawText(25, 35, "GHI", 0, &temp);
      break;
    case (1):
      drawText(25, 25, "ABC", 0, &temp);
      drawText(25, 30, "DEF", 1, &temp);
      drawText(25, 35, "GHI", 0, &temp);
      break;
    case(2):
      drawText(25, 25, "ABC", 0, &temp);
      drawText(25, 30, "DEF", 0, &temp);
      drawText(25, 35, "GHI", 1, &temp);
      break;
    }*/

    // --- UPDATE SEGMENT --- //
    //KEYBOARD INPUT
    char in = getch();

    //Change actions based on which scene we are in for user input.
    switch (currentScene) {
    case(0):
      switch (in) {
      case(' '):
        currentScene++;
        break;
      }
      break;
    case(1):
      switch (in) {
      case(' '):
        currentScene--;
        break;
      case('w'):
        //if (FetchMapChar(&mainMap, Player.x + 25, Player.y - 24) != wall)
        Player.y -= 1;
        break;
      case('s'):
        //if (FetchMapChar(&mainMap, Player.x + 25, Player.y + 26) != wall)
        Player.y += 1;
        break;
      case('a'):
        //if (FetchMapChar(&mainMap, Player.x - 1, Player.y + 25) != wall)
        Player.x -= 1;
        break;
      case('d'):
        //if (FetchMapChar(&mainMap, Player.x + 1, Player.y + 25) != wall)
        Player.x += 1;
        break;
      }
      break;
    }
    //Regardless of which scene we are in, break and exit.
    if (in == 'q') {
      break;
    }
    // --- END UPDATE SEGMENT --- //

    // --- DRAW SEGMENT --- //
    // Clear Draw Screen
    clear();

    //Always Draw

    //Draw Objects based on current scene.
    switch (currentScene) {
    case(0):
      DrawAsciiFile(&skull0, 50 - (skull0.width / 2), 50 - (skull0.height / 2));

      //Drawing Block Text
      //drawChar('S', Player.x, Player.y);
      //drawChar('O', Player.x + 5, Player.y);
      //drawChar('A', Player.x + 10, Player.y);
      //drawChar('P', Player.x + 15, Player.y);
      break;
    case(1):
      DrawMap(mainMenuInsetWindow, &mainMap, Player.x, Player.y);
      break;
    }

    wattron(mainMenuInsetWindow, COLOR_PAIR(2));
    mvwprintw(mainMenuInsetWindow, Player.y + 25, Player.x + 25, "!");
    mvwprintw(mainMenuInsetWindow, Player.y + 26, Player.x+25, "%d", Player.y);
    wattroff(mainMenuInsetWindow, COLOR_PAIR(2));
    // --- END DRAW SEGMENT ---
    refresh();                    /* Print it on to the real screen */
  }

ENDGAME:

  nodelay(mainWindow, FALSE);
  getch();                      /* Wait for user input */
  endwin();                     /* End curses mode */

  return 0;
}

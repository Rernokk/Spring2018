#include <curses.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPELLPAGELIMIT 12

//Object declarations and definitions.
typedef struct spell {
  char name[16];
  int keyframes;
  int cost;
  int animHeight;
  int animWidth;
  char *** data;
} spell;

typedef struct playerObject {
  int x;
  int y;
  int currentHealth;
  int maxHealth;
  int currentMana;
  int maxMana;
  int spellCount;
  spell * spellbook[];
} player;

typedef struct enemyObject {
  int currentHealth;
  int maxHealth;
  int currentMana;
  int maxMana;
  int spellCount;
  spell * spellbook[];
} enemy;

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
void WDrawAsciiFile(image * obj, int startX, int startY, WINDOW* win) {
  for (int i = 0; i < obj->height; i++) {
    mvwprintw(win, startY + i, startX, obj->data[i]);
  }
}
void DrawMap(WINDOW* win, image * map, int playerX, int playerY) {
  int winDim = getmaxx(win);
  for (int i = -winDim; i <= winDim; i++) {
    for (int j = -winDim; j <= winDim; j++) {
      if ((i + playerY < 0 || i + playerY >= map->height) || (j + playerX < 0 || j + playerX >= map->width)) {
        mvwprintw(win, i, j, " ");
      }
      else {
        char * c = map->data[i + playerY][j + playerX];
        mvwprintw(win, i, j, &c);
      }
    }
    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, ceil(winDim / 2), ceil(winDim / 2), "X");
    wattroff(win, COLOR_PAIR(2));
  }
}
void BorderWindow(WINDOW* win, char val) {
  for (int i = 0; i < getmaxy(win); i++) {
    for (int j = 0; j < getmaxx(win); j++) {
      if (i == 0 || j == 0 || i == getmaxy(win) - 1 || j == getmaxx(win) - 1) {
        mvwprintw(win, i, j, "#");
      }
    }
  }
}
void CenterPtWindow(WINDOW* win) {
  int maxX = floor(getmaxy(win)), maxY = floor(getmaxx(win));
  mvwprintw(win, maxX / 2, maxY / 2, "X");
}
char FetchMapChar(WINDOW* win, image * map, int tarX, int tarY) {
  int offSet = ceil(getmaxx(win) / 2);
  if (tarX + offSet >= 0 && tarY + offSet >= 0 && tarX + offSet < map->height && tarY + offSet < map->width) {
    return map->data[tarY + offSet][tarX + offSet];
  }
  return '@';
}
void AnimateTestSequence(int * state, int * isAnimating, spell* animatedSpell, WINDOW* win, int startX, int startY, int * animationCounter, int animationDelay, int loop, int pingpong, int * dir) {
  for (int i = 0; i < animatedSpell->animHeight; i++) {
    if (*state >= animatedSpell->keyframes) {
      if (pingpong == 0 && loop == 1) {
        *state = 0;
      }
      else if (pingpong == 1 && loop == 1) {
        (*dir) *= -1;
        *state += *dir;
      }
      if (loop == 0) {
        *isAnimating = 0;
        return;
      }
    }
    mvwprintw(win, startX + i, startY, animatedSpell->data[*state][i]);
  }
  if (*animationCounter > animationDelay) {
    *animationCounter = 0;
    if (*state == 0 && *dir == -1) {
      *dir = 1;
    }
    *state += *dir;
  }
  else {
    (*animationCounter)++;
  }
}
void LoadSpellData(spell * fileName) {
  fileName->data = (char***)malloc(sizeof(char***) * fileName->keyframes);
  for (int z = 0; z < fileName->keyframes; z++) {
    char temp[32];
    sprintf(temp, "%s%d.txt", fileName->name, z);

    printf("Loading Spell %s from ASCII...\n", temp);
    FILE* io = fopen(temp, "r");

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
    rewind(io);
    int fileHeight = 0;
    fileName->data[z] = (char**)malloc(sizeof(char**) * (size / lineLength + 2));
    for (int i = 0; i < (size / (lineLength + 2)); i++) {
      fileName->data[z][i] = (char*)malloc(sizeof(char*) * (lineLength + 1));
      fGetLine(io, fileName->data[z][i]);
      fileHeight++;
    }
    fileName->animHeight = fileHeight;
    fclose(io);
    printf("Loading %s, complete.\n", temp);
  }
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
  // --- Splash --- //
  WINDOW* mainMenuInsetWindow = subwin(mainWindow, 60, 60, 3, 3);

  // --- Combat --- //
  WINDOW* combatSceneSubWindow = subwin(mainWindow, 75, 75, 0, 0);
  WINDOW* combatMenuSubWindow = subwin(mainWindow, 26, 75, 75, 0);
  WINDOW* combatEnemyStatsSubWindow = subwin(mainWindow, 26, 26, 0, 75);
  WINDOW* combatMyStatsSubWindow = subwin(mainWindow, 26, 26, 75, 75);
  WINDOW* combatMySpellsSubWindow = subwin(mainWindow, 20, 26, 55, 75);
  WINDOW* combatEnemySpellsSubWindow = subwin(mainWindow, 20, 26, 26, 75);
  WINDOW* combatDecorationSubWindow = subwin(mainWindow, 11, 26, 45, 75);

  //Spell Initialization & Preloading
  spell fireball = { "demo", 2, 10 };
  LoadSpellData(&fireball);

  //Object Initialization and variables.
  player Player = { -10, -10, 100, 100, 100, 100, 2, {&fireball, &fireball} };
  enemy MyEnemy = { 100, 100, 100, 100, 3, {&fireball, &fireball, &fireball} };

  //Animation Variables
  int animatingSpell = 0;
  int animationState = 0;
  int animationDelay = 150;
  int animationCounter = 0;

  /* Scene Definitions & Purposes
     0 - Main Menu, player interacts w/ Scene to start game via Up/Down or W/S.
     1 - World Scene, player navigates 2D realm, overall HUD.
     2 - Combat Scene, player uses turn-based combat.
  */
  
  // --- Misc. Additional Variables --- //
  int currentScene = 0;
  int selectedMenu = 0;
  int temp = 0;
  int playerSpellPage = 0;
  int enemySpellPage = 0;
  int combatMenuX = 0, combatMenuY = 0;

  //Pre-loading artwork.
  spell skull = { "Skull", 7, 10 };
  image phoenix = { "Phoenix.txt" };
  image mainMap = { "mainMap.txt" };
  image spellDecoration = { "SpellDecoration.txt" };
  image actionDecoration = { "ActionDecoration.txt" };

  LoadSpellData(&skull);
  int skullDir = 1;
  LoadAsciiFile(&phoenix);
  LoadAsciiFile(&mainMap);
  LoadAsciiFile(&spellDecoration);
  LoadAsciiFile(&actionDecoration);

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
      switch (in)
      {
      case(' '):
        currentScene--;
        break;
      case('w'):
        if (FetchMapChar(mainMenuInsetWindow, &mainMap, Player.x, Player.y - 1) != wall)
          Player.y -= 1;
        break;
      case('s'):
        if (FetchMapChar(mainMenuInsetWindow, &mainMap, Player.x, Player.y + 1) != wall)
          Player.y += 1;
        break;
      case('a'):
        if (FetchMapChar(mainMenuInsetWindow, &mainMap, Player.x - 1, Player.y) != wall)
          Player.x -= 1;
        break;
      case('d'):
        if (FetchMapChar(mainMenuInsetWindow, &mainMap, Player.x + 1, Player.y) != wall)
          Player.x += 1;
        break;
      }
      break;
    case(2):
      switch (in) {
      case('i'):
        if (playerSpellPage > 0) {
          playerSpellPage -= 1;
        }
        break;
      case('k'):
        if (playerSpellPage < Player.spellCount - 1) {
          playerSpellPage += 1;
        }
        break;
      case('o'):
        if (enemySpellPage > 0) {
          enemySpellPage -= 1;
        }
        break;
      case('l'):
        if (enemySpellPage < MyEnemy.spellCount - 1) {
          enemySpellPage += 1;
        }
        break;

      case('w'):
        combatMenuX = abs(combatMenuX - 1);
        break;

      case('s'):
        combatMenuX = -combatMenuX + 1;
        break;
      case('a'):
        combatMenuY = abs(combatMenuY - 1);
        break;
      case('d'):
        combatMenuY = -combatMenuY + 1;
        break;

      case('g'):
        animatingSpell = 1;
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
      //DrawAsciiFile(&skull0, ceil(mainWindowMaxY / 2) - (skull0.width / 2), ceil(mainWindowMaxX / 2) - (skull0.height / 2));
      AnimateTestSequence(&animationState, &animatingSpell, &skull, mainWindow, 10, 10, &animationCounter, animationDelay*3, 1, 1, &skullDir);
      break;
    case(1):
      DrawMap(mainMenuInsetWindow, &mainMap, Player.x, Player.y);
      break;
    case(2):
      //Scene Borders
      BorderWindow(combatEnemySpellsSubWindow, "#");
      BorderWindow(combatEnemyStatsSubWindow, "#");
      BorderWindow(combatMenuSubWindow, "#");
      BorderWindow(combatMyStatsSubWindow, "#");
      BorderWindow(combatMySpellsSubWindow, "#");
      BorderWindow(combatSceneSubWindow, "#");
      BorderWindow(combatDecorationSubWindow, "#");

      //Misc UI Decorations
      WDrawAsciiFile(&spellDecoration, 1, 1, combatDecorationSubWindow);
      WDrawAsciiFile(&actionDecoration, 1, 1, combatMenuSubWindow);

      //Spell Book Population
      //Player
      mvwprintw(combatMySpellsSubWindow, 2, 8, "SPELL BOOK");
      for (int i = 0; i < (SPELLPAGELIMIT > Player.spellCount - playerSpellPage ? Player.spellCount - playerSpellPage : SPELLPAGELIMIT); i++) {
        mvwprintw(combatMySpellsSubWindow, 4 + i, 2, (*(Player.spellbook[playerSpellPage + i])).name);
        mvwprintw(combatMySpellsSubWindow, 4 + i, getmaxx(combatMySpellsSubWindow) - 4, "%d", (*(Player.spellbook[playerSpellPage + i])).cost);
      }
      mvwprintw(combatMySpellsSubWindow, getmaxy(combatMySpellsSubWindow) - 3, 2, "I -> Up \\/\\/ K -> Down");

      //Enemy
      mvwprintw(combatEnemySpellsSubWindow, 2, 8, "SPELL BOOK");
      for (int i = 0; i < (SPELLPAGELIMIT > MyEnemy.spellCount - enemySpellPage ? MyEnemy.spellCount - enemySpellPage : SPELLPAGELIMIT); i++) {
        mvwprintw(combatEnemySpellsSubWindow, 4 + i, 2, (MyEnemy.spellbook[enemySpellPage + i])->name);
        mvwprintw(combatEnemySpellsSubWindow, 4 + i, getmaxx(combatEnemySpellsSubWindow) - 4, "%d", (*(MyEnemy.spellbook[enemySpellPage + i])).cost);
      }
      mvwprintw(combatEnemySpellsSubWindow, getmaxy(combatEnemySpellsSubWindow) - 3, 2, "O -> Up /\\/\\ L -> Down");

      //Player Combat Details
      mvwprintw(combatMyStatsSubWindow, 2, 6, "Combat Details");
      mvwprintw(combatMyStatsSubWindow, 5, 3, "Health: %d / %d", Player.currentHealth, Player.maxHealth);
      for (int i = 0; i < floor(Player.currentHealth / Player.maxHealth) * 18; i++) {
        mvwprintw(combatMyStatsSubWindow, 7, 4 + i, "X");
      }
      mvwprintw(combatMyStatsSubWindow, 10, 3, "Mana: %d / %d", Player.currentHealth, Player.maxHealth);
      for (int i = 0; i < floor(Player.currentMana / Player.maxMana) * 18; i++) {
        mvwprintw(combatMyStatsSubWindow, 12, 4 + i, "X");
      }

      //Enemy Combat Details
      mvwprintw(combatEnemyStatsSubWindow, 2, 6, "Combat Details");
      mvwprintw(combatEnemyStatsSubWindow, 5, 3, "Health: %d / %d", MyEnemy.currentHealth, MyEnemy.maxHealth);
      for (int i = 0; i < floor(MyEnemy.currentHealth / MyEnemy.maxHealth) * 18; i++) {
        mvwprintw(combatEnemyStatsSubWindow, 7, 4 + i, "X");
      }
      mvwprintw(combatEnemyStatsSubWindow, 10, 3, "Mana: %d / %d", MyEnemy.currentHealth, MyEnemy.maxHealth);
      for (int i = 0; i < floor(MyEnemy.currentMana / MyEnemy.maxMana) * 18; i++) {
        mvwprintw(combatEnemyStatsSubWindow, 12, 4 + i, "X");
      }

      //Player Menu Selection
      if (combatMenuX == 0 && combatMenuY == 0) {
        wattron(combatMenuSubWindow, COLOR_PAIR(2));
        mvwprintw(combatMenuSubWindow, 7, 35, "ATTACK");
        wattroff(combatMenuSubWindow, COLOR_PAIR(2));
      }
      else {
        mvwprintw(combatMenuSubWindow, 7, 35, "ATTACK");
      }

      if (combatMenuX == 1 && combatMenuY == 0) {
        wattron(combatMenuSubWindow, COLOR_PAIR(2));
        mvwprintw(combatMenuSubWindow, 15, 35, "SPELL");
        wattroff(combatMenuSubWindow, COLOR_PAIR(2));
      }
      else {
        mvwprintw(combatMenuSubWindow, 15, 35, "SPELL");
      }

      if (combatMenuX == 0 && combatMenuY == 1) {
        wattron(combatMenuSubWindow, COLOR_PAIR(2));
        mvwprintw(combatMenuSubWindow, 7, 55, "ITEM");
        wattroff(combatMenuSubWindow, COLOR_PAIR(2));
      }
      else {
        mvwprintw(combatMenuSubWindow, 7, 55, "ITEM");
      }

      if (combatMenuX == 1 && combatMenuY == 1) {
        wattron(combatMenuSubWindow, COLOR_PAIR(2));
        mvwprintw(combatMenuSubWindow, 15, 55, "FLEE");
        wattroff(combatMenuSubWindow, COLOR_PAIR(2));
      }
      else {
        mvwprintw(combatMenuSubWindow, 15, 55, "FLEE");
      }

      //Animating Spell Cast
      if (animatingSpell == 1) {
        AnimateTestSequence(&animationState, &animatingSpell, &fireball, combatSceneSubWindow, 20, 20, &animationCounter, animationDelay, 1, 0, 1);
      }
      break;

    }
    // --- END DRAW SEGMENT --- //

    refresh();                    /* Print it on to the real screen */
  }

ENDGAME:

  nodelay(mainWindow, FALSE);
  getch();                      /* Wait for user input */
  endwin();                     /* End curses mode */

  return 0;
}

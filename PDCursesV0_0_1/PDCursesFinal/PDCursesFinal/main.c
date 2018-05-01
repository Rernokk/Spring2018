#include <curses.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SPELL_PAGE_LIMIT 12
#define ENEMY_COUNT 32
#define UPDATE_LIMITER 245
#define NAME_RANGE 25
#define SPELL_NUMBER 1
#define ITEM_MENU_OPTION_MAX 5
#define ATTACK_MENU_OPTION_MAX 4
#define SPELL_MENU_OPTION_MAX 4
#define FLEE_MENU_OPTION_MAX 1

//Object declarations and definitions.
typedef struct spell {
  char name[16];
  int keyframes;
  int cost;
  int animHeight;
  int animWidth;
  char *** data;
  char *** colData;
} spell;

typedef struct playerObject {
  char* name;
  int x;
  int y;
  int currentHealth;
  int maxHealth;
  int currentMana;
  int maxMana;
  int spellCount;
  int atkPower;
  int defPower;
  int healthPotions;
  int magicPotions;
  int atkAmpPotions;
  int defAmpPotions;
  int revives;
  spell * spellbook[];
} player;

typedef struct drawObject {
  char name[32];
  int width;
  int height;
  char ** data;
  char ** colData;
} image;

typedef struct enemyObject {
  char * name;
  int x;
  int y;
  int currentHealth;
  int maxHealth;
  int currentMana;
  int maxMana;
  int spellCount;
  int timeSinceLastUpdate;
  int level;
  double strikeWeakness;
  double slashWeakness;
  double lungeWeakness;
  int atkPower;
  int defPower;
  spell ** spellbook;
  image * myself;
} enemy;

typedef struct enemyLinkedListNode {
  struct enemyLinkedListNode * next;
  enemy* obj;
} ListNode;

//Accessible Methods For Various Purposes.
void DrawBox(int tlX, int tlY, int height, int width, int hollow) {
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
void DrawChar(char val, int posX, int posY) {
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
void DrawMenu(int sel) {
  DrawBox(1, 1, 98, 98, TRUE);
  DrawBox(3, 3, 94, 94, TRUE);
  //drawBox(25, 25, 10, 10, TRUE);
}
void DrawText(int startX, int startY, char* text, int highlighted, int* ptr) {
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
  char newFileTar[64];
  sprintf(newFileTar, "%s.txt", fileName->name);
  printf("Loading %s from ASCII...\n", newFileTar);
  FILE* io;
  io = fopen(newFileTar, "r");
  if (io == NULL) {
    return;
  }
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
  fileName->data = (char**)malloc(sizeof(char**) * (size / lineLength));
  for (int i = 0; i < (size / (lineLength)); i++) {
    fileName->data[i] = (char*)malloc(sizeof(char) * 2 * (lineLength + 1));
    fGetLine(io, fileName->data[i]);
  }
  fileName->width = lineLength;
  fileName->height = (size / (lineLength)) - 1;
  fclose(io);
  printf("Loading %s, complete.\n", fileName->name);
}
void LoadAsciiColors(image * fileName) {
  printf("Loading %s colors from ASCII...\n", fileName->name);
  char newFileTar[64];
  sprintf(newFileTar, "%s_color.txt", fileName->name);
  FILE* io;
  io = fopen(newFileTar, "r");
  if (io == NULL) {
    return;
  }
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
  fileName->colData = (char**)malloc(sizeof(char**) * (size / lineLength));
  for (int i = 0; i < (size / (lineLength)); i++) {
    fileName->colData[i] = (char*)malloc(sizeof(char) * 2 * (lineLength + 1));
    fGetLine(io, fileName->colData[i]);
  }
  //fileName->width = lineLength;
  //fileName->height = (size / (lineLength)) - 1;
  fclose(io);
  printf("Loading %s, complete.\n", fileName->name);
}
void DrawAsciiFile(image * obj, int startX, int startY) {
  for (int i = 0; i < obj->height; i++) {
    for (int j = 0; j < obj->width; j++) {
      if (obj->data[i][j] != " ") {
        /*wattron(win, COLOR_PAIR(colorpairvalue));
        for (int j = 0; j < animatedSpell->animWidth; j++) {
          if (animatedSpell->data[*state][i][j] != ' ') {
            mvwprintw(win, startX + i, startY + j, "%c", (animatedSpell->data[*state][i])[j]);
          }
        }
        wattroff(win, COLOR_PAIR(colorpairvalue));*/
        if (obj->colData != 0x00000000) {
          attron(COLOR_PAIR(atoi("%d", obj->colData[i][j])));
        }
        mvprintw(startY + i, startX + j, "%c", obj->data[i][j]);
        if (obj->colData != 0x00000000) {
          attroff(COLOR_PAIR(atoi("%d", obj->colData[i][j])));
        }
      }
    }
  }
}
void WDrawAsciiFile(image * obj, int startX, int startY, WINDOW* win) {
  for (int i = 0; i < obj->height; i++) {
    for (int j = 0; j < obj->width; j++) {
      if (obj->data[i][j] != " ") {
        if (obj->colData != 0x00000000 && obj->colData != 0xCDCDCDCD) {
          char c = obj->colData[i][j];
          int val = c - '0';
          wattron(win, COLOR_PAIR(val));
        }
        mvwprintw(win, startY + i, startX + j, "%c", obj->data[i][j]);
        if (obj->colData != 0x00000000 && obj->colData != 0xCDCDCDCD) {
          char c = obj->colData[i][j];
          int val = c - '0';
          wattroff(win, COLOR_PAIR(val));
        }
      }
    }
  }
}
void DrawMap(WINDOW* win, image * map, int playerX, int playerY) {
  int winDimX = getmaxx(win);
  int winDimY = getmaxy(win);
  for (int i = 0; i <= winDimY; i++) {
    for (int j = 0; j <= winDimX; j++) {
      if ((i + playerY - (int)(ceil(winDimY / 2)) < 0 || i + playerY - winDimY / 2 >= map->height) || (j + playerX - winDimX / 2 < 0 || j + playerX - winDimX / 2 >= map->width)) {
        mvwprintw(win, i, j, " ");
      }
      else {
        char * c = map->data[i + playerY - (int)(ceil(winDimY / 2))][j + playerX - (int)(ceil(winDimX / 2))];
        mvwprintw(win, i, j, &c);
      }
    }

    wattron(win, COLOR_PAIR(3));
    mvwprintw(win, ceil(winDimY / 2), ceil(winDimX / 2), "X");
    wattroff(win, COLOR_PAIR(3));
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
  int offSet = 0;
  if (tarX >= 0 && tarY >= 0 && tarY < map->height && tarX < map->width) {
    return map->data[tarY][tarX];
  }
  return '?';
}
void AnimateTestSequence(int * state, int * isAnimating, spell* animatedSpell, WINDOW* win, int startX, int startY, int * animationCounter, int animationDelay, int colorpairvalue) {
  for (int i = 0; i < animatedSpell->animHeight; i++) {
    for (int j = 0; j < animatedSpell->animWidth; j++) {
      char c = animatedSpell->colData[*state][i][j];
      wattron(win, COLOR_PAIR(c - '0'));
      if (animatedSpell->data[*state][i][j] != ' ') {
        mvwprintw(win, startX + i, startY + j, "%c", (animatedSpell->data[*state][i])[j]);
      }
      wattroff(win, COLOR_PAIR(c - '0'));
    }
  }
  if (*animationCounter > animationDelay) {
    *animationCounter = 0;
    *state += 1;
    if (*state >= animatedSpell->keyframes) {
      *isAnimating = 0;
      *state = 0;
    }
  }
  else {
    (*animationCounter)++;
  }
}
void LoadAnimationFile(spell * fileName) {
  fileName->data = (char***)malloc(sizeof(char***) * fileName->keyframes);
  for (int z = 0; z < fileName->keyframes; z++) {
    char temp[64];

    sprintf(temp, "%s%d.txt", fileName->name, z);

    FILE* io;
    io = fopen(temp, "r");
    if (io == NULL) {
      return;
    }

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
      fileName->data[z][i] = (char*)malloc(sizeof(char*) * 2 * (lineLength + 1));
      fGetLine(io, fileName->data[z][i]);
      fileHeight++;
    }
    fileName->animHeight = fileHeight;
    fileName->animWidth = (size / (lineLength + 2));
    fclose(io);
    printf("Loading %s, complete.\n", temp);
  }
}
void LoadAnimataionColor(spell * fileName) {
  fileName->colData = (char***)malloc(sizeof(char***) * fileName->keyframes);
  for (int z = 0; z < fileName->keyframes; z++) {
    char temp[64];

    sprintf(temp, "%s_%d_color.txt", fileName->name, z);

    FILE* io;
    io = fopen(temp, "r");
    if (io == NULL) {
      return;
    }

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
    fileName->colData[z] = (char**)malloc(sizeof(char**) * (size / lineLength + 2));
    for (int i = 0; i < (size / (lineLength + 2)); i++) {
      fileName->colData[z][i] = (char*)malloc(sizeof(char*) * 2 * (lineLength + 1));
      fGetLine(io, fileName->colData[z][i]);
      fileHeight++;
    }
    //fileName->animHeight = fileHeight;
    //fileName->animWidth = (size / (lineLength + 2));
    fclose(io);
    printf("Loading %s, complete.\n", temp);
  }
}
void DrawPlayerNameCenter(player obj, WINDOW* win) {
  mvwprintw(win, ceil(getmaxy(win) / 2) - 2, ceil(getmaxx(win) / 2) - floor(strlen(obj.name) / 2), obj.name);
}
void DrawEnemy(player * playerObj, enemy * obj, WINDOW* win) {
  if (obj->currentHealth < 0) {
    return;
  }
  int targetX = obj->y - playerObj->x + getmaxx(win) / 2;
  int targetY = obj->x - playerObj->y + getmaxy(win) / 2;
  mvwprintw(win, targetY, targetX, "Z");

  //Within Range for UI
  if (sqrt(pow((obj->y - playerObj->y), 2) + pow((obj->x - playerObj->x), 2)) < NAME_RANGE)
  {
    mvwprintw(win, targetY - 3, targetX - floor(strlen(obj->name) / 2), obj->name);
    wattron(win, COLOR_PAIR(9));
    mvwprintw(win, targetY - 2, targetX - floor(sizeof("%d/%d", obj->currentHealth, obj->maxHealth) / 2) - 1, "%d/%d", obj->currentHealth, obj->maxHealth);
    wattroff(win, COLOR_PAIR(9));
  }
}
void LoadEnemyData(ListNode * head) {
  //Static
  //*(enemyArray[0]) = ((enemy) { "Joblin", 12, 15, 100, 100, 100, 100, 3} );

  //Dynamic
  if (head->obj == NULL) {
    enemy* newHead = (enemy*)malloc(sizeof(enemy));
    newHead->name = "Goblin";
    newHead->x = 12;
    newHead->y = 12;
    newHead->currentHealth = newHead->currentMana = newHead->maxHealth = newHead->maxMana = 100;
    newHead->spellCount = 0;
    newHead->timeSinceLastUpdate = rand() % UPDATE_LIMITER;
    newHead->spellbook = (spell*)malloc(sizeof(spell*) * newHead->spellCount);
    newHead->level = 10;
    newHead->myself = (image*)malloc(sizeof(image));
    //char tempChar[32];
    //sprintf(tempChar, "%s.txt", newHead->name);
    strcpy(newHead->myself->name, newHead->name);
    LoadAsciiFile(newHead->myself);
    LoadAsciiColors(newHead->myself);
    newHead->slashWeakness = 15;
    newHead->lungeWeakness = 15;
    newHead->strikeWeakness = 12;
    newHead->atkPower = 10;
    newHead->defPower = 10;
    head->obj = newHead;
  }

  //Find the end of the list.
  ListNode * temp = head;
  while (temp->next != NULL) {
    temp = temp->next;
  }

  for (int i = 0; i < 5; i++) {
    temp->next = (ListNode*)malloc(sizeof(ListNode));
    temp = temp->next;
    enemy* myEnemy = (enemy*)malloc(sizeof(enemy));
    myEnemy->name = "Goblin";
    myEnemy->x = 13 + i;
    myEnemy->y = 12;
    myEnemy->currentHealth = myEnemy->currentMana = myEnemy->maxHealth = myEnemy->maxMana = 100;
    myEnemy->spellCount = 0;
    myEnemy->timeSinceLastUpdate = rand() % UPDATE_LIMITER;
    myEnemy->spellbook = (spell*)malloc(sizeof(spell*) * myEnemy->spellCount);
    myEnemy->level = 10;
    myEnemy->myself = (image*)malloc(sizeof(image));
    //char tempChar[32];
    //sprintf(tempChar, "%s.txt", myEnemy->name);
    //image tempImage = { tempChar };
    strcpy(myEnemy->myself->name, myEnemy->name);
    LoadAsciiFile(myEnemy->myself);
    LoadAsciiColors(myEnemy->myself);
    myEnemy->slashWeakness = .5;
    myEnemy->lungeWeakness = .75;
    myEnemy->strikeWeakness = 1.2;
    myEnemy->atkPower = 10;
    myEnemy->defPower = 10;
    myEnemy->currentHealth = 0;
    temp->obj = myEnemy;
  }

}
spell* FetchSpell(char* spellName, spell ** localSpellLibrary, int spellCount) {
  for (int i = 0; i < spellCount; i++) {
    if (strcmp(localSpellLibrary[i]->name, spellName) == 0) {
      return localSpellLibrary[i];
    }
  }
  return NULL;
}
char* GenerateConsumableDrop(player* playerObj) {
  int gen = rand() % (1 + 1);

  //And so we generate.
  if (gen == 0) {
    int upper = 100;
    int lower = 0;
    int gen2 = (rand() % (upper - lower + 1)) + lower;
    if (gen2 > 95) {
      playerObj->revives++;
      return "an Ancestral Spirit.";
    }
    else if (gen2 > 80) {
      playerObj->atkAmpPotions++;
      return "a Surge Reagent.";
    }
    else if (gen2 > 65) {
      playerObj->defAmpPotions++;
      return "a Warding Charm.";
    }
    else if (gen2 > 35) {
      int amnt = (rand() % (2 - 1 + 1)) + 1;
      playerObj->magicPotions += amnt;
      if (amnt > 1) {
        char* res = (char*)malloc(sizeof(char)*32);
        sprintf(res, "%d Mana Potions.", amnt);
        return res;
      }
      else {
        char* res = (char*)malloc(sizeof(char) * 32);
        sprintf(res, "%d Mana Potion.", amnt);
        return res;
      }
    }
    else {
      int amnt = (rand() % (3 - 1 + 1)) + 1;
      playerObj->healthPotions += amnt;
      if (amnt > 1){
        char* res = (char*)malloc(sizeof(char) * 32);
        sprintf(res, "%d Health Potions.", amnt);
        return res;
      }
      else {
        char* res = (char*)malloc(sizeof(char) * 32);
        sprintf(res, "%d Health Potion.", amnt);
        return res;
      }
    }
  }
  return "";
}

//Main Game
int main()
{
#pragma region Misc_Startup
  srand(time(0));
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
  WINDOW* mainMenuInsetWindow = subwin(mainWindow, 60, 60, 3, 3);
#pragma endregion

#pragma region Colors
  //Defining Color Pairs & Color Initialization
  start_color();
  init_color(COLOR_CYAN, 1000, 0, 0);

  //Enemy Health Bar
  init_pair(1, COLOR_RED, COLOR_RED);

  //Player Health Bar
  init_pair(2, COLOR_GREEN, COLOR_GREEN);

  //Menu Colors
  init_pair(3, COLOR_CYAN, COLOR_BLACK);

  //Mana Bar
  init_pair(4, COLOR_BLUE, COLOR_BLUE);

  //Red/Black
  init_pair(5, COLOR_RED, COLOR_BLACK);

  //Yellow/Black
  init_pair(6, COLOR_YELLOW, COLOR_BLACK);

  //Cyan/Black
  init_color(COLOR_CYAN, 1000, 500, 0);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);

  //Black/Black
  init_pair(8, COLOR_BLACK, COLOR_BLACK);

  //Red/White
  init_pair(9, COLOR_RED, COLOR_WHITE);

#pragma endregion

#pragma region Combat_Windows
  WINDOW* combatSceneSubWindow = subwin(mainWindow, 75, 75, 0, 0);
  WINDOW* combatMenuSubWindow = subwin(mainWindow, 26, 75, 75, 0);
  WINDOW* combatEnemyStatsSubWindow = subwin(mainWindow, 26, 26, 0, 75);
  WINDOW* combatMyStatsSubWindow = subwin(mainWindow, 26, 26, 75, 75);
  WINDOW* combatMySpellsSubWindow = subwin(mainWindow, 20, 26, 55, 75);
  WINDOW* combatEnemySpellsSubWindow = subwin(mainWindow, 20, 26, 26, 75);
  WINDOW* combatDecorationSubWindow = subwin(mainWindow, 11, 26, 46, 75);
#pragma endregion

#pragma region World_Windows
  WINDOW* worldSceneGameWindow = subwin(mainWindow, 81, 101, 0, 0);
  WINDOW* worldInfoGameWindow = subwin(mainWindow, 20, 101, 81, 0);
#pragma endregion

#pragma region Spells
  printf("Loading Spells.\n");
  spell* spellLibrary[SPELL_NUMBER];
  spellLibrary[0] = &((spell) { "Flameblast", 9, 10 });

  printf("Beginning Loading Spell Animations.\n");
  for (int i = 0; i < SPELL_NUMBER; i++) {
    LoadAnimationFile(spellLibrary[i]);
    LoadAnimataionColor(spellLibrary[i]);
  }
#pragma endregion

#pragma region MyEnemy&Player
  //Object Initialization and variables.
  player Player = { "Rernokk", 10, 10, 100, 100, 100, 100, 2, 10, 10, 1, 0, 1, 1, 0, {FetchSpell("Flameblast", spellLibrary, SPELL_NUMBER), FetchSpell("Flameblast", spellLibrary, SPELL_NUMBER) } };
  //enemy MyEnemy = { "Placeholder", 12, 15, 100, 100, 100, 100, 1, 0 };
  enemy* MyEnemy = NULL;
  /*MyEnemy.spellbook = (spell*)malloc(sizeof(spell*));
  MyEnemy.spellbook[0] = FetchSpell("demo", spellLibrary, 4);*/
#pragma endregion

#pragma region Animation_Variables
  int animatingSpell = 0;
  int animationState = 0;
  int animationDelay = 50;
  int animationCounter = 0;
#pragma endregion

#pragma region Scenes
  /* Scene Definitions & Purposes
     0 - Main Menu, player interacts w/ Scene to start game via Up/Down or W/S.
     1 - World Scene, player navigates 2D realm, overall HUD.
     2 - Combat Scene, player uses turn-based combat.
     3 - Backstory Scene
     4 - Instructions
     5 - Credits
  */
#pragma endregion

#pragma region Misc
  int currentScene = 0;

  int selectedMenu = 0;
  int itemMenu = ITEM_MENU_OPTION_MAX;
  int spellMenu = SPELL_MENU_OPTION_MAX;
  int attackMenu = ATTACK_MENU_OPTION_MAX;
  int fleeMenu = 0;

  int atkBuffDur = 0;
  int defBuffDur = 0;

  int temp = 0;
  int playerSpellPage = 0;
  int enemySpellPage = 0;
  int combatMenuX = 0, combatMenuY = 0;
  char* combatMenuActionSubWindow = "MAIN";
  int turn = 0;
#pragma endregion

#pragma region Artwork
  //Pre-loading artwork.
  //spell skull = { "Skull", 7, 10 };
  image phoenix = { "Phoenix" };
  image mainMap = { "mainMap" };
  image spellDecoration = { "SpellDecoration" };
  image actionDecoration = { "ActionDecoration" };
  image splashArt = { "Splash" };

  //LoadAnimationFile(&skull);
  int skullDir = 1;
  LoadAsciiFile(&phoenix);
  LoadAsciiFile(&mainMap);
  LoadAsciiFile(&spellDecoration);
  LoadAsciiFile(&actionDecoration);
  LoadAsciiFile(&splashArt);

  //LoadAsciiColors(&spellDecoration);
  //LoadAsciiColors(&actionDecoration);
  actionDecoration.height += 1;
#pragma endregion

#pragma region Enemies
  //Dynamic
  ListNode enemyListHead = { NULL, NULL };
  LoadEnemyData(&enemyListHead);
  for (int i = 0; i < enemyListHead.obj->spellCount; i++) {
    //enemyListHead.obj->spellbook[i] = FetchSpell("Fireball", &spellLibrary);
  }
  //MyEnemy = *enemyListHead.obj;
#pragma endregion

  //Denoting ASCII Symbols
  char wall = '#';
  //Game Loop
  while (TRUE) {
#pragma region Update_Loop
    // --- UPDATE SEGMENT --- //
    //KEYBOARD INPUT
    char in = getch();

    //Change actions based on which scene we are in for user input.
    switch (currentScene) {
#pragma region Main Menu Controls
    case(0):
      switch (in) {
      case('w'):
        selectedMenu -= 1;
        if (selectedMenu < 0) {
          selectedMenu = 4;
        }
        break;
      case('s'):
        selectedMenu++;
        if (selectedMenu > 4) {
          selectedMenu = 0;
        }
        break;
      case('z'):
        currentScene = 2;
        break;
      case(' '):
        switch (selectedMenu) {
        case(0):
          //Gameplay
          currentScene = 1;
          break;
        case(1):
          //Instructions
          currentScene = 4;
          break;
        case(2):
          //Credits
          currentScene = 5;
          break;
        case(3):
          //Backstory
          currentScene = 3;
          break;
        case(4):
          goto ENDGAME;
          break;
        }
        break;
      }
      break;
#pragma endregion

#pragma region World Scene Controls
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

#pragma region AIUpdateOverworld
      ListNode* temp = &enemyListHead;
      while (temp != 0xCDCDCDCD) {
        if (temp->obj->currentHealth > 0) {
          if (temp->obj->timeSinceLastUpdate > UPDATE_LIMITER) {

            int myDir = (rand() % 3) - 1;
            switch (rand() % 2)
            {
            case(0):
              if (FetchMapChar(worldSceneGameWindow, &mainMap, temp->obj->y + myDir, temp->obj->x) != wall) {
                temp->obj->y += myDir;
              }
              break;
            case(1):
              if (FetchMapChar(worldSceneGameWindow, &mainMap, temp->obj->y, temp->obj->x + myDir) != wall) {
                temp->obj->x += myDir;
              }
              break;
            }
            temp->obj->timeSinceLastUpdate = 0;
          }
          else {
            temp->obj->timeSinceLastUpdate++;
          }

          if (temp->obj->x == Player.y && temp->obj->y == Player.x) {
            MyEnemy = temp->obj;
            currentScene = 2;
          }
        }
        temp = temp->next;

      }
      break;
#pragma endregion
#pragma endregion

#pragma region Combat Scene Controls
    case(2):
      if (turn == 0) {
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
          if (enemySpellPage < MyEnemy->spellCount - 1) {
            enemySpellPage += 1;
          }
          break;

#pragma region Menu Controls
        case('w'):
          //Main
          if (combatMenuActionSubWindow == "MAIN")
          {
            combatMenuX = abs(combatMenuX - 1);
          }

          //Item
          else if (combatMenuActionSubWindow == "ITEM")
          {
            itemMenu += 1;
            if (itemMenu > ITEM_MENU_OPTION_MAX) {
              itemMenu = 0;
            }
          }

          //Spell
          else if (combatMenuActionSubWindow == "SPELL") {
            spellMenu += 1;
            if (spellMenu > SPELL_MENU_OPTION_MAX) {
              spellMenu = 0;
            }
          }

          //Flee
          else if (combatMenuActionSubWindow == "FLEE") {
            fleeMenu += 1;
            if (fleeMenu > FLEE_MENU_OPTION_MAX) {
              fleeMenu = 0;
            }
          }

          //Attack
          else if (combatMenuActionSubWindow == "ATTACK") {
            attackMenu += 1;
            if (attackMenu > ATTACK_MENU_OPTION_MAX) {
              attackMenu = 0;
            }
          }
          break;

        case('s'):
          //Main
          if (combatMenuActionSubWindow == "MAIN") {
            combatMenuX = -combatMenuX + 1;
          }

          //Item
          else if (combatMenuActionSubWindow == "ITEM")
          {
            itemMenu -= 1;
            if (itemMenu < 0) {
              itemMenu = ITEM_MENU_OPTION_MAX;
            }
          }

          //Spell
          else if (combatMenuActionSubWindow == "SPELL") {
            spellMenu -= 1;
            if (spellMenu < 0) {
              spellMenu = SPELL_MENU_OPTION_MAX;
            }
          }

          //Flee
          else if (combatMenuActionSubWindow == "FLEE") {
            fleeMenu -= 1;
            if (fleeMenu < 0) {
              fleeMenu = FLEE_MENU_OPTION_MAX;
            }
          }

          //Attack
          else if (combatMenuActionSubWindow == "ATTACK") {
            attackMenu -= 1;
            if (attackMenu < 0) {
              attackMenu = ATTACK_MENU_OPTION_MAX;
            }
          }

          break;

        case('a'):
          //Main
          if (combatMenuActionSubWindow == "MAIN")
            combatMenuY = abs(combatMenuY - 1);
          break;
        case('d'):
          //Main
          if (combatMenuActionSubWindow == "MAIN")
            combatMenuY = -combatMenuY + 1;
          break;
        case(' '):
#pragma region MainUpdate
          if (combatMenuActionSubWindow == "MAIN") {
            if (combatMenuX == 0) {
              switch (combatMenuY) {
              case(0):
                combatMenuActionSubWindow = "ATTACK";
                break;
              case(1):
                combatMenuActionSubWindow = "SPELL";
                break;
              }
            }
            else {
              switch (combatMenuY) {
              case(0):
                combatMenuActionSubWindow = "ITEM";
                break;
              case(1):
                combatMenuActionSubWindow = "FLEE";
                break;
              }
            }
#pragma endregion
          }
          else if (combatMenuActionSubWindow == "ATTACK") {
#pragma region AttackUpdate
            int dmgAmnt = 0;
            if (atkBuffDur > 0) {
              dmgAmnt = floor((1.25*Player.atkPower) / (1 + log10(MyEnemy->defPower)));
            }
            else {
              dmgAmnt = floor(Player.atkPower / (1 + log10(MyEnemy->defPower)));
            }
            switch (attackMenu) {
            case (4):
              dmgAmnt = floor(dmgAmnt * MyEnemy->strikeWeakness);
              MyEnemy->currentHealth -= dmgAmnt;
              nodelay(mainWindow, FALSE);
              wclear(combatMenuSubWindow);
              BorderWindow(combatMenuSubWindow, "#");
              WDrawAsciiFile(&actionDecoration, 1, 1, combatMenuSubWindow);
              mvwprintw(combatMenuSubWindow, 4, 32, "You strike %s, dealing %d damage!", MyEnemy->name, dmgAmnt);
              wrefresh(combatMenuSubWindow);
              getch();
              if (MyEnemy->currentHealth > 0) {
                turn = abs(turn - 1);
                nodelay(mainWindow, TRUE);
              }
              else {
                MyEnemy->currentHealth = 0;
                char* drop = GenerateConsumableDrop(&Player);
                wclear(combatMenuSubWindow);
                BorderWindow(combatMenuSubWindow, "#");
                WDrawAsciiFile(&actionDecoration, 1, 1, combatMenuSubWindow);
                mvwprintw(combatMenuSubWindow, 4, 32, "You've killed the %s!", MyEnemy->name);
                if (drop != ""){
                  mvwprintw(combatMenuSubWindow, 6, 32, "You've found %s", drop);
                }
                wrefresh(combatMenuSubWindow);
                getch();
                nodelay(mainWindow, TRUE);
                currentScene = 1;
              }
              break;
            case (3):
              dmgAmnt = floor(dmgAmnt * MyEnemy->slashWeakness);
              MyEnemy->currentHealth -= dmgAmnt;
              turn = abs(turn - 1);
              nodelay(mainWindow, FALSE);
              wclear(combatMenuSubWindow);
              BorderWindow(combatMenuSubWindow, "#");
              WDrawAsciiFile(&actionDecoration, 1, 1, combatMenuSubWindow);
              mvwprintw(combatMenuSubWindow, 4, 32, "You slash at %s, dealing %d damage!", MyEnemy->name, dmgAmnt);
              wrefresh(combatMenuSubWindow);
              getch();
              if (MyEnemy->currentHealth > 0) {
                turn = abs(turn - 1);
                nodelay(mainWindow, TRUE);
              }
              else {
                MyEnemy->currentHealth = 0;
                char* drop = GenerateConsumableDrop(&Player);
                wclear(combatMenuSubWindow);
                BorderWindow(combatMenuSubWindow, "#");
                WDrawAsciiFile(&actionDecoration, 1, 1, combatMenuSubWindow);
                mvwprintw(combatMenuSubWindow, 4, 32, "You've killed the %s!", MyEnemy->name);
                if (drop != "") {
                  mvwprintw(combatMenuSubWindow, 6, 32, "You've found %s", drop);
                }
                wrefresh(combatMenuSubWindow);
                getch();
                nodelay(mainWindow, TRUE);
                currentScene = 1;
              }
              break;
            case (2):
              dmgAmnt = floor(dmgAmnt * MyEnemy->lungeWeakness);
              MyEnemy->currentHealth -= dmgAmnt;
              turn = abs(turn - 1);
              nodelay(mainWindow, FALSE);
              wclear(combatMenuSubWindow);
              BorderWindow(combatMenuSubWindow, "#");
              WDrawAsciiFile(&actionDecoration, 1, 1, combatMenuSubWindow);
              mvwprintw(combatMenuSubWindow, 4, 32, "You lunge at %s, dealing %d damage!", MyEnemy->name, dmgAmnt);
              wrefresh(combatMenuSubWindow);
              getch();
              if (MyEnemy->currentHealth > 0) {
                turn = abs(turn - 1);
                nodelay(mainWindow, TRUE);
              }
              else {
                MyEnemy->currentHealth = 0;
                char* drop = GenerateConsumableDrop(&Player);
                wclear(combatMenuSubWindow);
                BorderWindow(combatMenuSubWindow, "#");
                WDrawAsciiFile(&actionDecoration, 1, 1, combatMenuSubWindow);
                mvwprintw(combatMenuSubWindow, 4, 32, "You've killed the %s!", MyEnemy->name);
                if (drop != "") {
                  mvwprintw(combatMenuSubWindow, 6, 32, "You've found %s", drop);
                }
                wrefresh(combatMenuSubWindow);
                getch();
                nodelay(mainWindow, TRUE);
                currentScene = 1;
              }
              break;

            case(0):
              combatMenuActionSubWindow = "MAIN";
              break;
            }
#pragma endregion
          }
          else if (combatMenuActionSubWindow == "ITEM") {
#pragma region ItemUpdate
            switch (itemMenu) {
            case(5):
              if (Player.healthPotions > 0) {
                Player.healthPotions--;
                Player.currentHealth += floor(.25*Player.maxHealth);
                if (Player.currentHealth > Player.maxHealth) {
                  Player.currentHealth = Player.maxHealth;
                }
                turn = abs(turn - 1);
              }
              break;
            case(4):
              if (Player.magicPotions > 0) {
                Player.magicPotions--;
                Player.currentMana += floor(.25*Player.maxHealth);
                if (Player.currentMana > Player.maxMana) {
                  Player.currentMana = Player.maxMana;
                }
                turn = abs(turn - 1);
              }
            case(3):
              if (Player.atkAmpPotions > 0) {
                Player.atkAmpPotions--;
                atkBuffDur = 3;
                turn = abs(turn - 1);
              }
              break;
            case(2):
              if (Player.defAmpPotions > 0) {
                Player.defAmpPotions--;
                defBuffDur = 3;
                turn = abs(turn - 1);
              }
              break;
            case(1):
              if (Player.revives > 0) {
                Player.revives--;
                Player.currentHealth = (Player.maxHealth);
                turn = abs(turn - 1);
              }
              break;
            case(0):
              combatMenuActionSubWindow = "MAIN";
              break;
            }
#pragma endregion
          }
          else if (combatMenuActionSubWindow == "SPELL") {
#pragma region SpellUpdate
            switch (spellMenu) {
            case(0):
              combatMenuActionSubWindow = "MAIN";
              break;
            }
#pragma endregion
          }
          else if (combatMenuActionSubWindow == "FLEE") {
#pragma region FleeUpdate
            switch (fleeMenu) {
            case(1):
              combatMenuActionSubWindow = "MAIN";
              currentScene = 1;
              MyEnemy->x += rand() % (3 + 3 + 1) - 3;
              MyEnemy->y += rand() % (3 + 3 + 1) - 3;
            case(0):
              combatMenuActionSubWindow = "MAIN";
              break;
            }
#pragma endregion
          }
          break;
        case('g'):
          animatingSpell = 1;
          break;
        }


#pragma endregion
      }
      else {
#pragma region AIProcessing
        if (MyEnemy->name == "Goblin") {
          int dmgAmnt = 0;
          if (defBuffDur > 0) {
            dmgAmnt = floor(MyEnemy->atkPower / (1 + log10(floor(Player.defPower * 1.25))));
          }
          else {
            dmgAmnt = floor(MyEnemy->atkPower / (1 + log10(Player.defPower)));
          }
          Player.currentHealth -= dmgAmnt;
          turn = abs(turn - 1);
          nodelay(mainWindow, FALSE);
          wclear(combatMenuSubWindow);
          BorderWindow(combatMenuSubWindow, "#");
          WDrawAsciiFile(&actionDecoration, 1, 1, combatMenuSubWindow);
          mvwprintw(combatMenuSubWindow, 4, 32, "%s attacks you! You take %d damage!", MyEnemy->name, dmgAmnt);
          wrefresh(combatMenuSubWindow);
          getch();
          combatMenuActionSubWindow = "MAIN";
          nodelay(mainWindow, TRUE);
        }
        if (atkBuffDur > 0) {
          atkBuffDur--;
        }
        if (defBuffDur > 0) {
          defBuffDur--;
        }
#pragma endregion
      }
      break;
#pragma endregion

#pragma region Filler Scene Controls
    case(3):
    case(4):
    case(5):
      switch (in) {
      case(' '):
        currentScene = 0;
        break;
      }
      break;
    }
#pragma endregion

#pragma region Global Controls

    //Regardless of which scene we are in, break and exit.
    if (in == 'q') {
      break;
    }
#pragma endregion
    // --- END UPDATE SEGMENT --- //

#pragma endregion

#pragma region Draw_Loop

// --- DRAW SEGMENT --- //
// Clear Draw Screen
    clear();

    //Draw Objects based on current scene.
    switch (currentScene) {
#pragma region Draw Main Menu
    case(0):
      DrawAsciiFile(&splashArt, 0, 0);
      mvwprintw(mainWindow, 20, 2, "Created By Shannon Smith");
      mvwprintw(mainWindow, 21, 2, "CS2060-003");
      mvwprintw(mainWindow, 22, 2, "Spring 2018");
      mvwprintw(mainWindow, 23, 2, "April 14th, 2018");

      if (selectedMenu == 0) {
        wattron(mainWindow, COLOR_PAIR(3));
        mvwprintw(mainWindow, 60, 2, "Play Game");
        wattroff(mainWindow, COLOR_PAIR(3));
      }
      else {
        mvwprintw(mainWindow, 60, 2, "Play Game");
      }

      if (selectedMenu == 1) {
        wattron(mainWindow, COLOR_PAIR(3));
        mvwprintw(mainWindow, 62, 2, "Instructions");
        wattroff(mainWindow, COLOR_PAIR(3));
      }
      else {
        mvwprintw(mainWindow, 62, 2, "Instructions");
      }
      if (selectedMenu == 2) {
        wattron(mainWindow, COLOR_PAIR(3));
        mvwprintw(mainWindow, 64, 2, "Credits");
        wattroff(mainWindow, COLOR_PAIR(3));
      }
      else {
        mvwprintw(mainWindow, 64, 2, "Credits");
      }
      if (selectedMenu == 3) {
        wattron(mainWindow, COLOR_PAIR(3));
        mvwprintw(mainWindow, 66, 2, "Backstory");
        wattroff(mainWindow, COLOR_PAIR(3));
      }
      else {
        mvwprintw(mainWindow, 66, 2, "Backstory");
      }
      if (selectedMenu == 4) {
        wattron(mainWindow, COLOR_PAIR(3));
        mvwprintw(mainWindow, 68, 2, "Exit");
        wattroff(mainWindow, COLOR_PAIR(3));
      }
      else {
        mvwprintw(mainWindow, 68, 2, "Exit");
      }
      break;
#pragma endregion

#pragma region Draw World Scene
    case(1):
      DrawMap(worldSceneGameWindow, &mainMap, Player.x, Player.y);
      //Dynamic
      ListNode* temp = &enemyListHead;
      while (temp != 0xCDCDCDCD) {
        if (temp->obj->currentHealth > 0) {
          DrawEnemy(&Player, temp->obj, worldSceneGameWindow);
        }
        //mvwprintw(worldSceneGameWindow, Player.y - temp->obj->y , Player.x - temp->obj->x , "!");
        temp = temp->next;
        if (temp == NULL) {
          break;
        }
      }
      mvwprintw(worldInfoGameWindow, 5, 5, "Player X,Y: %d,%d. Up: %c", Player.x, Player.y, FetchMapChar(worldSceneGameWindow, &mainMap, Player.x, Player.y - 1));
      mvwprintw(worldInfoGameWindow, 6, 5, "%s X,Y: %d,%d. Up: %c", enemyListHead.obj->name, enemyListHead.obj->y, enemyListHead.obj->x, FetchMapChar(worldSceneGameWindow, &mainMap, enemyListHead.obj->y, enemyListHead.obj->x - 1));
      DrawPlayerNameCenter(Player, worldSceneGameWindow);
      wattron(worldSceneGameWindow, COLOR_PAIR(3));
      mvwprintw(worldSceneGameWindow, ceil(getmaxy(worldSceneGameWindow) / 2), ceil(getmaxx(worldSceneGameWindow) / 2), "X");
      wattroff(worldSceneGameWindow, COLOR_PAIR(3));
      BorderWindow(worldInfoGameWindow, "#");
      break;

#pragma endregion

#pragma region Draw Combat Scene
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

      //Drawing Current Enemy
      if (MyEnemy != NULL) {
        WDrawAsciiFile(MyEnemy->myself, 1, 1, combatSceneSubWindow);
      }

#pragma region HUDInterface
      //Spell Book Population
      //Player
      mvwprintw(combatMySpellsSubWindow, 3, 8, "SPELL BOOK");
      for (int i = 0; i < (SPELL_PAGE_LIMIT > Player.spellCount - playerSpellPage ? Player.spellCount - playerSpellPage : SPELL_PAGE_LIMIT); i++) {
        mvwprintw(combatMySpellsSubWindow, 5 + i, 2, (*(Player.spellbook[playerSpellPage + i])).name);
        mvwprintw(combatMySpellsSubWindow, 5 + i, getmaxx(combatMySpellsSubWindow) - 4, "%d", (*(Player.spellbook[playerSpellPage + i])).cost);
      }
      mvwprintw(combatMySpellsSubWindow, getmaxy(combatMySpellsSubWindow) - 3, 2, "I -> Up \\/\\/ K -> Down");

      //Enemy
      mvwprintw(combatEnemySpellsSubWindow, 2, 8, "SPELL BOOK");
      for (int i = 0; i < (SPELL_PAGE_LIMIT > MyEnemy->spellCount - enemySpellPage ? MyEnemy->spellCount - enemySpellPage : SPELL_PAGE_LIMIT); i++) {
        mvwprintw(combatEnemySpellsSubWindow, 4 + i, 2, (MyEnemy->spellbook[enemySpellPage + i])->name);
        mvwprintw(combatEnemySpellsSubWindow, 4 + i, getmaxx(combatEnemySpellsSubWindow) - 4, "%d", (*(MyEnemy->spellbook[enemySpellPage + i])).cost);
      }
      mvwprintw(combatEnemySpellsSubWindow, getmaxy(combatEnemySpellsSubWindow) - 3, 2, "O -> Up /\\/\\ L -> Down");

      //Player Combat Details
      mvwprintw(combatMyStatsSubWindow, 1, ceil(getmaxx(combatMyStatsSubWindow) / 2) - floor(strlen(Player.name) / 2), "%s", Player.name);
      mvwprintw(combatMyStatsSubWindow, 2, 6, "Combat Details");
      mvwprintw(combatMyStatsSubWindow, 5, 3, "Health: %d / %d", Player.currentHealth, Player.maxHealth);

      if (atkBuffDur > 0) {
        mvwprintw(combatMyStatsSubWindow, 15, 3, "Atk Power: %d", (int)floor(Player.atkPower * 1.25));
      }
      else {
        mvwprintw(combatMyStatsSubWindow, 15, 3, "Atk Power: %d", Player.atkPower);
      }

      if (defBuffDur > 0) {
        mvwprintw(combatMyStatsSubWindow, 17, 3, "Def Power: %d", (int)floor(Player.defPower * 1.25));
      }
      else {
        mvwprintw(combatMyStatsSubWindow, 17, 3, "Def Power: %d", Player.defPower);
      }
      wattron(combatMyStatsSubWindow, COLOR_PAIR(2));
      for (int i = 0; i < floor(((float)Player.currentHealth / Player.maxHealth) * 18); i++) {
        mvwprintw(combatMyStatsSubWindow, 7, 4 + i, "X");
      }
      wattroff(combatMyStatsSubWindow, COLOR_PAIR(2));
      mvwprintw(combatMyStatsSubWindow, 10, 3, "Mana: %d / %d", Player.currentMana, Player.maxMana);
      wattron(combatMyStatsSubWindow, COLOR_PAIR(4));
      for (int i = 0; i < floor(((float)Player.currentMana / Player.maxMana) * 18); i++) {
        mvwprintw(combatMyStatsSubWindow, 12, 4 + i, "X");
      }
      wattroff(combatMyStatsSubWindow, COLOR_PAIR(4));

      //Enemy Combat Details
      mvwprintw(combatEnemyStatsSubWindow, 1, ceil(getmaxx(combatEnemyStatsSubWindow) / 2) - floor(strlen(MyEnemy->name) / 2), "%s", MyEnemy->name);
      mvwprintw(combatEnemyStatsSubWindow, 2, 6, "Combat Details");
      mvwprintw(combatEnemyStatsSubWindow, 5, 3, "Health: %d / %d", MyEnemy->currentHealth, MyEnemy->maxHealth);
      mvwprintw(combatEnemyStatsSubWindow, 15, 3, "Atk Power: %d", MyEnemy->atkPower);
      mvwprintw(combatEnemyStatsSubWindow, 17, 3, "Def Power: %d", MyEnemy->defPower);
      wattron(combatEnemyStatsSubWindow, COLOR_PAIR(1));
      for (int i = 0; i < floor(((float)MyEnemy->currentHealth / MyEnemy->maxHealth) * 18); i++) {
        mvwprintw(combatEnemyStatsSubWindow, 7, 4 + i, "X");
      }
      wattroff(combatEnemyStatsSubWindow, COLOR_PAIR(1));

      mvwprintw(combatEnemyStatsSubWindow, 10, 3, "Mana: %d / %d", MyEnemy->currentMana, MyEnemy->maxMana);
      wattron(combatEnemyStatsSubWindow, COLOR_PAIR(4));
      for (int i = 0; i < floor(((float)MyEnemy->currentMana / MyEnemy->maxMana) * 18); i++) {
        mvwprintw(combatEnemyStatsSubWindow, 12, 4 + i, "X");
      }
      wattroff(combatEnemyStatsSubWindow, COLOR_PAIR(4));
#pragma endregion

      //Player Menu Selection
      if (combatMenuActionSubWindow == "MAIN") {
#pragma region MainMenu
        if (combatMenuX == 0 && combatMenuY == 0) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 7, 35, "ATTACK");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 7, 35, "ATTACK");
        }

        if (combatMenuX == 1 && combatMenuY == 0) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 15, 35, "ITEM");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 15, 35, "ITEM");
        }

        if (combatMenuX == 0 && combatMenuY == 1) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 7, 55, "SPELL");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 7, 55, "SPELL");
        }

        if (combatMenuX == 1 && combatMenuY == 1) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 15, 55, "FLEE");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 15, 55, "FLEE");
        }
#pragma endregion
      }
      else if (combatMenuActionSubWindow == "SPELL") {
#pragma region SpellMenu
        char spellInput[32];
        mvwprintw(combatMenuSubWindow, 2, 32, "CAST SPELL");

        if (spellMenu == 0) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, combatMenuSubWindow->_maxy - 3, 32, "Cancel Action");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, combatMenuSubWindow->_maxy - 3, 32, "Cancel Action");
        }
#pragma endregion
      }
      else if (combatMenuActionSubWindow == "ATTACK") {
#pragma region AttackMenu
        mvwprintw(combatMenuSubWindow, 2, 32, "PHYSICAL ATTACK");

        if (attackMenu == 4) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 5, 32, "Strike");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 5, 32, "Strike");
        }

        if (attackMenu == 3) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 7, 32, "Slash");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 7, 32, "Slash");
        }

        if (attackMenu == 2) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 9, 32, "Lunge");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 9, 32, "Lunge");
        }

        if (attackMenu == 1) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 11, 32, "Guard");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 11, 32, "Guard");
        }

        if (attackMenu == 0) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, combatMenuSubWindow->_maxy - 3, 32, "Cancel Action");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, combatMenuSubWindow->_maxy - 3, 32, "Cancel Action");
        }
#pragma endregion
      }
      else if (combatMenuActionSubWindow == "ITEM") {
#pragma region ItemMenu
        mvwprintw(combatMenuSubWindow, 2, 32, "USE ITEM");

        if (itemMenu == 5) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 5, 32, "Health Potions: %d", Player.healthPotions);
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 5, 32, "Health Potions: %d", Player.healthPotions);
        }

        if (itemMenu == 4) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 7, 32, "Mana Potions: %d", Player.magicPotions);
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 7, 32, "Mana Potions: %d", Player.magicPotions);
        }

        if (itemMenu == 3) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 9, 32, "Surge Reagent: %d", Player.atkAmpPotions);
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 9, 32, "Surge Reagent: %d", Player.atkAmpPotions);
        }

        if (itemMenu == 2) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 11, 32, "Warding Charm: %d", Player.defAmpPotions);
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 11, 32, "Warding Charm: %d", Player.defAmpPotions);
        }

        if (itemMenu == 1) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, 13, 32, "Ancestral Call: %d", Player.revives);
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, 13, 32, "Ancestral Call: %d", Player.revives);
        }

        if (itemMenu == 0) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, combatMenuSubWindow->_maxy - 3, 32, "Cancel Action");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, combatMenuSubWindow->_maxy - 3, 32, "Cancel Action");
        }
#pragma endregion
      }
      else if (combatMenuActionSubWindow == "FLEE") {
#pragma region FleeMenu
        mvwprintw(combatMenuSubWindow, 2, 32, "FLEE COMBAT");
        if (fleeMenu == 1) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, combatMenuSubWindow->_maxy - 5, 32, "Attempt to Flee");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, combatMenuSubWindow->_maxy - 5, 32, "Attempt to Flee");
        }

        if (fleeMenu == 0) {
          wattron(combatMenuSubWindow, COLOR_PAIR(3));
          mvwprintw(combatMenuSubWindow, combatMenuSubWindow->_maxy - 3, 32, "Cancel Action");
          wattroff(combatMenuSubWindow, COLOR_PAIR(3));
        }
        else {
          mvwprintw(combatMenuSubWindow, combatMenuSubWindow->_maxy - 3, 32, "Cancel Action");
        }
#pragma endregion
      }
      //Animating Spell Cast
      if (animatingSpell == 1) {
        //Currently defaults to flameblast.
        AnimateTestSequence(&animationState, &animatingSpell, FetchSpell("Flameblast", spellLibrary, SPELL_NUMBER), combatSceneSubWindow, 1, 1, &animationCounter, animationDelay, 5);
      }
      break;
#pragma endregion

#pragma region Draw Backstory
    case(3):
      mvprintw(1, 1, "Backstory - Press space to return to main menu.");
      mvprintw(5, 1, "This is still a work in progress. For now, you play as a rogue spellweaver.");
      mvprintw(6, 1, "You are wandering the country side in search of better reagents and ancient arcane powers.");
      mvprintw(8, 1, "In your journey, you will cross enemies...");
      mvprintw(9, 1, "Some may attempt to rob you, others will try to outright kill you.");
      mvprintw(10, 1, "Prove to the world that you are not to be taken lightly.");
      break;
#pragma endregion

#pragma region Draw Instructions
    case(4):
      mvprintw(1, 1, "Instructions - Press space to return to main menu.");
      mvprintw(5, 1, "While still a work in progress, you will have several areas to explore and\n find new equipment and arcane items.");
      mvprintw(8, 1, "Some people you cross will try to kill you, wherein you will fight them directly.");
      mvprintw(10, 1, "Combat is turn-based.");
      mvprintw(11, 1, "Attack with brute force and ancient magics to devastate your foes.");
      mvprintw(13, 1, "Traverse the open world with:");
      mvprintw(14, 1, "W - Up");
      mvprintw(15, 1, "A - Left");
      mvprintw(16, 1, "D - Right");
      mvprintw(17, 1, "S - Down");
      mvprintw(20, 1, "For now, you can explore the world by pressing play on the main screen.");
      mvprintw(21, 1, "You can also see the combat interface by pressing <Z> while in the main menu, and <Z> to return.");
      break;
#pragma endregion

#pragma region Draw Credits
    case(5):
      mvprintw(1, 1, "Credits - Press space to return to main menu.");
      mvwprintw(mainWindow, 5, 2, "Created By Shannon Smith");
      mvwprintw(mainWindow, 7, 2, "CS2060-003");
      mvwprintw(mainWindow, 9, 2, "Spring 2018");
      mvwprintw(mainWindow, 11, 2, "April 14th, 2018");
      break;
#pragma endregion
    }

    //Always Draw


    // --- END DRAW SEGMENT --- //

    refresh();                    /* Print it on to the real screen */
#pragma endregion
  }

ENDGAME:

  nodelay(mainWindow, FALSE);
  //getch();                      /* Wait for user input */
  endwin();                     /* End curses mode */

  return 0;
}

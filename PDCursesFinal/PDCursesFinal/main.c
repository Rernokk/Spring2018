#include "curses.h"
#include "math.h"

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

int main()
{
  WINDOW* win = initscr();                    /* Start curses mode */
  nodelay(win, TRUE);
  curs_set(0);
  resize_term(100, 100);
  if (has_colors == FALSE) {
    endwin();
    printf("No support for color");
    exit(1);
  }
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);

  int playerX = 8;
  int playerY = 8;
  //char playerRep = 'X';

  int selectedMenu = 0;

  //Game Loop
  while (TRUE) {
    clear();
    attron(COLOR_PAIR(1));
    //printw("Hello World!!!");    /* Print Hello World */
    //mvprintw(playerY, playerX, "%c", playerRep);
    drawMenu(selectedMenu);
    mvprintw(10, 46, "SOME BULLSHT");
    attroff(COLOR_PAIR(1));
    drawChar('P', playerX, playerY);
    drawChar('L', playerX + 5, playerY);
    drawChar('A', playerX + 10, playerY);
    drawChar('Y', playerX + 15, playerY);

    char in = getch();
    if (in == 'q') {
      break;
    }
    switch (in) {
    case('w'):
      playerY -= 1;
      break;

    case ('s'):
      playerY += 1;
      break;

    case ('a'):
      playerX -= 1;
      break;

    case ('d'):
      playerX += 1;
      break;
    }


    switch (in) {
    case ('w'):
      selectedMenu -= 1;
      if (selectedMenu < 0) {
        selectedMenu = 2;
      }
      break;
    }

    refresh();                    /* Print it on to the real screen */
  }

  nodelay(win, FALSE);
  getch();                      /* Wait for user input */
  endwin();                     /* End curses mode */

  return 0;
}
#include "curses.h"
#include "math.h"

int main()
{
  WINDOW* win = initscr();                    /* Start curses mode */
  nodelay(win, TRUE);
  curs_set(0);
  if (has_colors == FALSE) {
    endwin();
    printf("No support for color");
    exit(1);
  }
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  int playerX = 1;
  int playerY = 1;
  char playerRep = 'X';
  
  int selectedMenu = 0;

  //Game Loop
  while (TRUE) {
    clear();
    attron(COLOR_PAIR(1));
    printw("Hello World!!!");    /* Print Hello World */
    //mvprintw(playerY, playerX, "%c", playerRep);
    attroff(COLOR_PAIR(1));
    
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
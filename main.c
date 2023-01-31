#include <curses.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
int startup(int x, int y) {
  // window setup
  WINDOW *menu = newwin(10, x / 5, y / 2 - y / 4, x / 2 - x / 10);
  mvwprintw(menu, 0, 2, "Menu");
  mvwprintw(menu, 1, x / 10 - 5, "CHOOSE MODE");
  box(menu, 0, 0);
  keypad(menu, true);

  // Mode Menu
  int choice, highlight = 0;
  char opts[3][9] = {"Hardcore", "Medium", "Easy"};
  while (1) {
    for (int i = 0; i < 3; i++) {
      if (i == highlight)
        wattron(menu, A_REVERSE);
      mvwprintw(menu, i + 4, x / 10 - 4 + i, "%s", opts[i]);
      wattroff(menu, A_REVERSE);
    }
    choice = wgetch(menu);
    switch (choice) {
    case KEY_UP:
      highlight == 0 ? highlight = 2 : highlight--;
      break;
    case KEY_DOWN:
      highlight == 2 ? highlight = 0 : highlight++;
      break;
    default:
      break;
    }
    if (choice == 10) {
      break;
    }
    wrefresh(menu);
  }
  wrefresh(menu);
  return highlight;
}
void* timer() {
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *Timer = newwin(3, 16, 2, (int)x / 2 + x / 4);
  box(Timer, 0, 0);
  for (int i=1; i<61; ++i) {
    mvwprintw(Timer, 1, 1, "Time Passed:%d", i);
    wrefresh(Timer);
    sleep(1);
  }
  return 0;
}
int main(int argc, char *argv[]) {
  // Initializing screen
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  if (!has_colors()) {
    return -1;
  }
  noecho();
  int x, y, mode;
  getmaxyx(stdscr, y, x);
  //Starup Page
  mvprintw(4, x / 2 - 11, "Welcome To Type Rush!!\n");
  refresh();
  mode = startup(x, y);
  clear();

  //Main Screen
  WINDOW *win = newwin(7, x / 2, 5, (int)x / 2 - x / 4);
  refresh();
  box(win, 0, 0);
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  attron(COLOR_PAIR(1));
  mvwprintw(win, 0, x / 4 - 5, "Sample Text");
  attroff(COLOR_PAIR(1));
  wmove(win, 1, 1);
  wrefresh(win);
  //Using Multithreading to run timer in background
  pthread_t Time_thread;
  pthread_create(&Time_thread, NULL, timer, NULL);
  sleep(2);
  mvwprintw(win, 3, x/4-5, "Sample Text");
  wrefresh(win);    
  getch();
  endwin();
  return 0;
}

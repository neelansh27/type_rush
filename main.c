#include <curses.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int game=1;
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
  WINDOW *Timer = newwin(3, 17, 2, (int)x / 2 + x / 4);
  box(Timer, 0, 0);
  for (int i=60; i>=0; --i) {
    curs_set(0);
    i<10 ? mvwprintw(Timer, 1, 1, "Time Left: %ds ", i):mvwprintw(Timer, 1, 1, "Time Left: %ds ", i);
    wrefresh(Timer);
    sleep(1);
  }
  game=0;
  return 0;
}
int main() {
  // Initializing screen
  initscr();
  noecho();
  curs_set(0);
  if (!has_colors()) {
    return -1;
  }
  start_color();
  noecho();
  int x, y, mode;
  getmaxyx(stdscr, y, x);
  //Starup Page
  mvprintw(4, x / 2 - 11, "Welcome To Type Rush!!\n");
  refresh();
  mode = startup(x, y);
  clear();
  char dummy_text[]="Far far away, behind the word mountains, far from the countries Vokalia and Consonantia, there live the blind texts. Separated they live in Bookmarksgrove right at the coast of the Semantics, a large language ocean. A small river named Duden flows by their place and supplies it with the necessary regelialia. It is a paradisematic country, in which roasted parts of sentences fly into your mouth. Even the all-powerful Pointing has no control about the blind texts it is an almost unorthographic life One day however a small line of blind text by the name of Lorem Ipsum decided to leave for the far World of Grammar. The Big Oxmox advised her not to do so, because there were thousands of bad Commas, wild Question Marks and devious Semikoli, but the Little Blind Text didn’t listen. She packed her seven versalia, put her initial into the belt and made herself on the way. When she reached the first hills of the Italic Mountains, she had a last view back on the skyline of her hometown Bookmarksgrove, the headline of Alphabet Village and the subline of her own road, the Line Lane. Pityful a rethoric question ran over her cheek, then ";
  //Main Screen
  WINDOW *win = newwin(22, x / 2, 5, x / 2 - x / 4);
  refresh();
  box(win, 0, 0);
  keypad(win , true );

  //Color Palatte
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, 255, 000);
  init_pair(3, 111, 000);

  wattron(win,COLOR_PAIR(1));
  wattron(win,A_BOLD);
  mvwprintw(win, 0, x / 4 - 5, "Start Typing");
  wattroff(win,A_BOLD);
  wattroff(win,COLOR_PAIR(1));
  wmove(win, 1, 1);
  wrefresh(win);
  //Using Multithreading to run timer in background
  pthread_t Time_thread;
  pthread_create(&Time_thread, NULL, timer, NULL);
  int row=1,col=1;
  wattron(win, COLOR_PAIR(2));
  for (int i = 0; i < strlen(dummy_text);i++) {
      if(col>x/2-2){
          wmove(win, ++row, col=1);
        continue;
      }
      mvwprintw(win, row, col, "%c",dummy_text[i]);
      col++;
  }
  wattroff(win, COLOR_PAIR(2));
  wrefresh(win);
  //USER INPUT
  int index=0,display_text;
  char input[2000];
  row=1;col=1;
  while ((display_text=wgetch(win))!=27 || game ) {
    curs_set(1);
    if (col==(x/2-1)){
      wmove(win, ++row, col=1);
    }
    switch (display_text) {
      case KEY_BACKSPACE:
        if (col>1){
          wattron(win, COLOR_PAIR(2));
          col--;
          mvwprintw(win , row,col, "%c\b",dummy_text[(row-1)*(x/2-1)+col-1]);
          wattroff(win, COLOR_PAIR(3));
          input[--index]='\0';
        }
        wrefresh(win);
        break;
      case '\n':
      case KEY_UP:
      case KEY_DOWN:
      case KEY_END:
      case KEY_HOME:
      case KEY_RIGHT:
      case KEY_LEFT:
      case KEY_DL:
      case KEY_SDL:
        break;
      default:
     if (row<=20) {
        wattron(win, COLOR_PAIR(3));
        input[index]=display_text;
        mvwprintw(win , row, col, "%c",display_text);
        wattroff(win, COLOR_PAIR(3));
        wrefresh(win);
        col++;
        index++;
        }
      }
  }
  endwin();
  printf("%s",input);
  return 0;
}

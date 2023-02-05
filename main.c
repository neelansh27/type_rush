#include <curses.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int game=1; //GLOBAL VARIABLE FOR GAME STATE
float total_time;

int startup(int x, int y);
void* timer();
int total_charac_by_user(char user_input[]);
int errors(char text[], char user_input[]);
float gross_wpm(int len_text, float total_time);
float net_wpm(int total_charac_by_user, int error, float total_time);
float accuracy(float net_wpm, float gross_wpm);

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
  char* dummy_text[3]={
  "! $%& /() =?* '<> #|;abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ ! $%& /() =?* '<> #|; ed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo invent !$%& /() =?* '<> #|; {} abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ !$%& /() =?* '<> #|;{} em quibusdam et aut officiis debitis aut rerum necessitatibus saepe eveniet ut et voluptates repudiandae sint et molestiae non recusandae. Itaque earum r!$%& /() =?* '<> #|;{} abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV bWXYZ ! $%& /() =?* '<> #|; {} abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ ! $%& /() =?* '<> #| abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ ! $%& /() =?* '<> #| {} abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ !%& /() =? '<> #|; {} abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ  $%& /() =?* '<> #|; abc def ghi jkl mno pqrs tuv",
  "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas nec odio et ante tincidunt tempus. Donec ",
  "Far far away, behind the word mountains, far from the countries Vokalia and Consonantia, there live the blind texts. Separated they live in Bookmarksgrove right at the coast of the Semantics, a large language ocean. A small river named Duden flows by their place and supplies it with the necessary regelialia. It is a paradisematic country, in which roasted parts of sentences fly into your mouth. Even the all-powerful Pointing has no control about the blind texts it is an almost unorthographic life One day however a small line of blind text by the name of Lorem Ipsum decided to leave for the far World of Grammar. The Big Oxmox advised her not to do so, because there were thousands of bad Commas, wild Question Marks and devious Semikoli, but the Little Blind Text didn't listen. She packed her seven versalia, put her initial into the belt and made herself on the way. When she reached the first hills of the Italic Mountains, she had a last view back on the skyline of her hometown Bookmarksgrove, the headline of Alphabet Village and the subline of her own road, the Line Lane. Pityful a rethoric question ran over her cheek, then "};
  //Main Screen
  WINDOW *win = newwin(22, x / 2, 5, x / 2 - x / 4);
  refresh();
  box(win, 0, 0);
  keypad(win , true );

  //Color Palatte
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, 255, 000);
  init_pair(3, 111, 000);
  init_pair(4, COLOR_RED, COLOR_BLACK);

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
  for (int i = 0; i < strlen(dummy_text[mode]);i++) {
      if(col>x/2-2){
        wmove(win, ++row, col=1);
      }
      mvwprintw(win, row, col, "%c",dummy_text[mode][i]);
      col++;
  }
  wattroff(win, COLOR_PAIR(2));
  wrefresh(win);
  //USER INPUT
  int index=0,display_text;
  char input[2000];
  input[0]='\0';
  row=1;col=1;
  while ((display_text=wgetch(win))!=27) {
    if (!game){
      break;
    }
    curs_set(1);
    if (col==(x/2-1)){
      wmove(win, ++row, col=1);
    }
    switch (display_text) {
      case KEY_BACKSPACE:
        if (col>1){
          wattron(win, COLOR_PAIR(2));
          col--;
          mvwprintw(win , row,col, "%c\b",dummy_text[mode][(row-1)*(x/2-2)+col-1]); //Formula for replacing with orignal text
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
        input[++index]='\0';
        mvwprintw(win , row, col, "%c",display_text);
        wattroff(win, COLOR_PAIR(3));
        wrefresh(win);
        col++;
      }
    }
  }
  //Stopping the time if user ended the game abruptly
  pthread_cancel(Time_thread);

  //Fetching Statistics
  int char_len,err;
  float g_wpm,n_wpm,acc;
  char_len=strlen(input);
  total_time= (total_time<=0) ? 1: 1-(total_time/60);//Time will become -1 after last iteration
  err=errors(dummy_text[mode], input);
  n_wpm=net_wpm(char_len,err, total_time);
  g_wpm=gross_wpm(char_len,total_time);
  acc=accuracy(n_wpm, g_wpm);

  //Displaying Statistics
  clear();
  WINDOW* res= newwin(8,18,y/2-6, x/2-9);
  refresh();
  box(res, 0, 0);
  mvwprintw(res, 0, 4, "Statistics");
  mvwprintw(res, 2, 1, "NET WPM  :%.2f",n_wpm);
  mvwprintw(res, 3, 1, "GROSS WPM:%.2f",g_wpm);
  mvwprintw(res, 4, 1, "ERRORS   :%d",err);
  mvwprintw(res, 5, 1, "ACCURACY :%.2f",acc);
  wrefresh(res);
  attron(COLOR_PAIR(4));
  attron(A_BOLD);
  mvprintw(y/2+4, x/2-13, "Press any key twice to quit");
  attroff(A_BOLD);
  attroff(COLOR_PAIR(4));
  refresh();
  getch();
  getch();
  endwin(); //Clear the screen and exit
  return 0;
}

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
  for (total_time = 60; total_time>=0; --total_time) {
    curs_set(0);
    total_time<10 ? mvwprintw(Timer, 1, 1, "Time Left: %.0fs ", total_time):mvwprintw(Timer, 1, 1, "Time Left: %.0fs ", total_time);
    wrefresh(Timer);
    sleep(1);
  }
  game=0;
  return 0;
}

int total_charac_by_user(char user_input[])
{
    int text_length = strlen(user_input);
    int count = 0;
    for (int x = 0; x < text_length; x++)
    {
        if (user_input[x] == ' ')
        {
            count += 1;
        }
    }
    int len_text = text_length - count;
    return len_text;
}
int errors(char text[], char user_input[])
{
    int len = strlen(user_input);
    int error = 0;
    for (int l = 0; l < len; l++)
    {
        if (text[l]!=user_input[l])
        {
            error += 1;
        }
    }
    return error;
}

float gross_wpm(int len_text, float ttime)
{
    if (!len_text){
      return 0;
    }
    float words = (float)len_text / 5;
    float words_per_min = words/ttime;
    return words_per_min;
}

float net_wpm(int len_text, int error, float ttime)
{
    float correct_word =  (float)(len_text-error)/  5;
    float net_wpm = correct_word / ttime;
    return net_wpm;
}
float accuracy(float net_wpm, float gross_wpm)
{
    if (gross_wpm==0){
      return 0;
    }
    float per_acc = (net_wpm/gross_wpm) * 100;
    return per_acc;
}

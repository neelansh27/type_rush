#include <curses.h>
#include <stdio.h>
#include <ncurses.h>
int main (int argc, char *argv[])
{
    initscr(); 
    cbreak();
    noecho();
    curs_set(0);
    if (!has_colors()) {
    return -1;
    }
    noecho();
    int x,y;
    getmaxyx(stdscr, y, x);
    mvprintw(4,x/2-11,"Welcome To Type Rush!!\n");
    refresh();
    //Window Setup
    WINDOW* menu=newwin(10, x/5, y/2-y/4, x/2-x/10);
    mvwprintw(menu, 0, 2, "Menu");
    mvwprintw(menu, 1, x/10-5, "CHOOSE MODE");
    box(menu, 0, 0);   
    keypad(menu, true);

    int choice,highlight=0;
    char opts[3][9]={"Hardcore","Medium","Easy"};
    while (1)
    {
        for (int i = 0; i < 3; i++) {
            if (i==highlight) 
                wattron(menu, A_REVERSE);
            mvwprintw(menu, i+4, x/10-4+i,"%s",opts[i]);
            wattroff(menu, A_REVERSE);
        }
        choice=wgetch(menu);    
        switch (choice) {
            case KEY_UP:
                highlight==0? highlight=2:highlight--;
                break;
            case KEY_DOWN:
                highlight==2? highlight=0:highlight++;
                break;
            default:
                break;
        }
        if (choice==10) {
            break;
        }
        wrefresh(menu);
    }
    wrefresh(menu);
    endwin();
    return 0;
}

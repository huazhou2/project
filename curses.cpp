//#include<iostream>
#include <curses.h>
//#include <unistd.h>
using namespace std;
int main(int argc, char *argv[]) {
  initscr();
	noecho();
	curs_set(FALSE);
	WINDOW *win=newwin(10,10,1,1);
	box(win,'.',' ');
	touchwin(win);
	wrefresh(win);
	getchar();
	endwin();
    } 

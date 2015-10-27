#include <ncurses.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;
void initialize();//initialize game pane
void init_game(WINDOW*, int&, int&); 	
void init_screen(WINDOW* mywin,int& gap_row,int& gap_col); 
void show_game(WINDOW* mywin,const vector<vector<int> >& board,int, int); 
void rotate_left(vector<vector<int> >& board);
void rotate_right(vector<vector<int> >& board);
int move_left(vector<vector<int> >& board);
void generate_new(vector<vector<int> >& board);
void new_game(WINDOW* gamewin,WINDOW* statwin,vector<vector<int> >& board,int,int);
bool is_Gameover(vector<vector<int> >& board);
void game_over(WINDOW*);
int main(int argc, char *argv[]) {
	initialize();
	int std_y, std_x;
	getmaxyx(stdscr,std_y,std_x);
	int win_size;
	const int start_x=4,start_y=4;
	if (std_y<20) 
		win_size=std_y/3;
	else 	
		win_size=20;	
	WINDOW * game_win=newwin(win_size,2*win_size,start_y,start_x);
	WINDOW * stat_win=newwin(win_size,2*win_size,start_y+win_size,start_x);
	int gap_row,gap_col;
	init_screen(game_win,gap_row,gap_col);
	vector<vector<int> > board(4,vector<int>(4,0));//create board matrix
	//start and initialize color mode
	start_color();
	init_pair(2,COLOR_BLUE,COLOR_BLACK);
	init_pair(3,COLOR_GREEN,COLOR_BLACK);
	init_pair(4,COLOR_YELLOW,COLOR_BLACK);
	init_pair(5,COLOR_CYAN,COLOR_BLACK);
	init_pair(6,COLOR_MAGENTA,COLOR_BLACK);
	init_pair(7,COLOR_RED,COLOR_BLACK);
	new_game(game_win,stat_win,board,gap_row,gap_col);
	//read in game input
	keypad(game_win,true);
	int key;
	int score=0;
	//game play
	while(1) {
		int get_score=0;//the score you get in one move
		show_game(game_win,board,gap_row,gap_col);
		wrefresh(game_win);	
		key=wgetch(game_win);
		if (is_Gameover(board)) { //game over, only accept q or 1 as input
			game_over(stat_win);	
			if (key=='q') break;
			else if (key=='1') 
			new_game(game_win,stat_win,board,gap_row,gap_col);}
		else {	//regular game play with direction key etc.	
			switch(key) {
			case KEY_UP:
				rotate_left(board);
				get_score=move_left(board);
				rotate_right(board);
				break;
			case KEY_DOWN:
				rotate_right(board);
				get_score=move_left(board);
				rotate_left(board);
				break;
			case KEY_LEFT:
				get_score=move_left(board);
				break;
			case KEY_RIGHT:
				rotate_right(board);
				rotate_right(board);
				get_score=move_left(board);
				rotate_left(board);
				rotate_left(board);
				break;
			case '1':
				new_game(game_win,stat_win,board,gap_row,gap_col);
				break;
				}
			}
		if (key=='q') break;
		if (get_score) {//print new score if scored
			score+=get_score;
			wattron(stat_win,COLOR_PAIR(6));
			mvwprintw(stat_win,4,1,"      ");
			mvwprintw(stat_win,4,1,"Score %6d",score);
			wattroff(stat_win,COLOR_PAIR(4));
			wrefresh(stat_win);
			}
		}
	endwin();
	}
void initialize() {
	initscr();
	noecho();
	curs_set(0);
	cbreak();
	nonl();
	}

void init_screen(WINDOW* mywin, int &gap_row, int &gap_col) {	
	int y,x;
	getmaxyx(mywin,y,x);
	double scale=1.0f;//scale factor to change board size
	gap_row=(y-2)/4*scale;
	gap_col=x/6*scale;
	for (int i=0;i<=4*gap_row;i=i+gap_row) {
		for (int j=0;j<=4*gap_col;j++) 
			mvwaddch(mywin,i,j,'-');
	}
	for (int i=0;i<=4*gap_col;i=i+gap_col) {
		for (int j=0;j<=4*gap_row;j++) 
			if (j==0 || j%gap_row==0)
			mvwaddch(mywin,j,i,'+');
			else
			mvwaddch(mywin,j,i,'|');
		}	
	}
//show the current board on game screen
void show_game(WINDOW* mywin,const vector<vector<int> >& board, int gap_row, int gap_col) {
	int row=board.size();
	int col=board[0].size();
	//clear screen
	for (int i=0;i<row;i++) 
		for (int j=0;j<col;j++)  
		mvwprintw(mywin,gap_row*i+gap_row/2,gap_col*j+gap_col/4,"    ");
	wrefresh(mywin);
	start_color();
	for (int i=0;i<row;i++) 
		for (int j=0;j<col;j++)  
		if (board[i][j]!=0) {
		int digit=0;
		int number=board[i][j];
		while (number) {
			number>>=1;
			digit++;}
		int color;
		color=digit/2+1;
		if (digit>13) color=7;//max color being 7
		wattron(mywin,COLOR_PAIR(color));
		wattron(mywin,A_BOLD);
		mvwprintw(mywin,gap_row*i+gap_row/2,gap_col*j+gap_col/4,"%4d",board[i][j]);
		wattroff(mywin,COLOR_PAIR(color));
		wattroff(mywin,A_BOLD);
			}
	}

int move_left (vector<vector<int> >& board) {//move and return score you get
	int row=board.size();
	int col=board[0].size();
	int zero_count=0;
	bool is_moved=false;
	int score=0;
	//combine the same number and set the cell to right as 0
	for (int i=0;i<row;i++) {
		for (int j=0;j<col-1;j++) 
			if (board[i][j] && board[i][j]==board[i][j+1]) { 
				is_moved=true;
				score+=board[i][j];
				board[i][j]<<=1;
				board[i][j+1]=0;
				}
		}
	//shift left if 0 to its left
	for (int i=0;i<row;i++) { 
		int cur=0;
		for (int j=0;j<col;j++) {
			if (board[i][j]!=0) { 
				if (j!=cur) { 
				is_moved=true;
				board[i][cur]=board[i][j];
				board[i][j]=0;}
				cur++;}
				}
		zero_count=zero_count+board.size()-cur;
			}
	if (is_moved) {//if any move then generate 2 at random pos 
		int pos=rand()%zero_count+1;
		int count=0;
		for (int i=0;i<board.size();i++) 
			for (int j=0;j<board[i].size();j++) {
				if (!board[i][j]) {
					count++;
					if (count==pos) board[i][j]=2;}
					}
		}
	return score;
	}
void rotate_left (vector<vector<int> >& board) {
	int row=board.size();
	int col=board[0].size();
	//mirrow by upper left -> bottom right diagonal
	for (int i=0;i<row-1;i++) {
		for (int j=i+1;j<col;j++) {
			int temp=board[i][j];
			board[i][j]=board[j][i];
			board[j][i]=temp; }
		}
	//mirrow upside down
	for (int i=0;i<row/2;i++) {
		for (int j=0;j<col;j++) {
			int temp=board[i][j];
			board[i][j]=board[row-i-1][j];
			board[row-i-1][j]=temp;
				}
			}
	}
void rotate_right (vector<vector<int> >& board) {
	rotate_left(board);
	rotate_left(board);
	rotate_left(board);
		}
void generate_new(vector<vector<int> >& board) {//generate game board
	srand(time(NULL));
	int pos1, pos2;
	do {
	pos1=rand()%16;
	pos2=rand()%16;
	} while (pos1==pos2);
	for (int i=0;i<board.size();i++) 
		for (int j=0;j<board[i].size();j++) 
			if (i*board.size()+j==pos1) 
			board[i][j]=2;
			else if (i*board.size()+j==pos2)
			board[i][j]=rand()%2?2:4;
			else board[i][j]=0;
	}
void new_game(WINDOW* game_win,WINDOW* stat_win,vector<vector<int> >& board,int gap_row, int gap_col) {
	wclear(stat_win);
	wrefresh(stat_win);
	start_color();
	wattron(stat_win,COLOR_PAIR(7));
	mvwprintw(stat_win,1,1,"Hua's 2048 Game");
	wattroff(stat_win,COLOR_PAIR(2));
	wattron(stat_win,COLOR_PAIR(3));
	mvwprintw(stat_win,2,1,"Use Arrow Key to control move");
	mvwprintw(stat_win,3,1,"Press Q to quit");
	wattroff(stat_win,COLOR_PAIR(3));
	wattron(stat_win,COLOR_PAIR(6));
	mvwprintw(stat_win,4,1,"Score %6d",0);
	wattroff(stat_win,COLOR_PAIR(4));
	wrefresh(stat_win);
	generate_new(board);
	show_game(game_win, board,gap_row,gap_col);
	}
void game_over(WINDOW* stat_win) {
	start_color();
	wattron(stat_win,COLOR_PAIR(7));
	wattron(stat_win,A_BLINK);
	wattron(stat_win,A_BOLD);
	mvwprintw(stat_win,5,1,"Game Over");
	wattroff(stat_win,A_BLINK);
	wattroff(stat_win,A_BOLD);
	mvwprintw(stat_win,6,1,"q for quit and 1 for new game");
	wrefresh(stat_win);
	}	
bool is_Gameover(vector<vector<int> >& board) {
	for (int i=0;i<board.size();i++) {
		for (int j=0;j<board[i].size()-1;j++) {
			if (!board[i][j]) return false; 
			//compare with next row
			if (i+1<board.size() && board[i][j]==board[i+1][j]) return false; 
			if (j+1<board[i].size() && board[i][j]==board[i][j+1]) return false;	}
	}
	return true;
	}

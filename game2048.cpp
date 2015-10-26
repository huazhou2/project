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
	
//	mvwprintw(game_win,1,1,"game window");
	start_color();
	int score=0;
	new_game(game_win,stat_win,board,gap_row,gap_col);
	
//read in game input
	keypad(game_win,true);
	int key;
	while(1) {
		if (is_Gameover(board)) break;	
		key=wgetch(game_win);
		int get_score=0;
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
		if (key=='q') break;
		show_game(game_win,board,gap_row,gap_col);
		wrefresh(game_win);	
		if (get_score) {
			score+=get_score;
			wattron(stat_win,COLOR_PAIR(4));
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
	for (int i=0;i<row;i++) 
		for (int j=0;j<col;j++)  
		if (board[i][j]!=0)
		mvwprintw(mywin,gap_row*i+gap_row/2,gap_col*j+gap_col/4,"%4d",board[i][j]);
	}

int move_left (vector<vector<int> >& board) {
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
	//shift left if 0 to left
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
	if (is_moved) {
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
void generate_new(vector<vector<int> >& board) {
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
	start_color();
	init_pair(2,COLOR_RED,COLOR_BLACK);
	wattron(stat_win,COLOR_PAIR(2));
	mvwprintw(stat_win,1,1,"Hua's 2048 Game");
	init_pair(3,COLOR_GREEN,COLOR_BLACK);
	wattron(stat_win,COLOR_PAIR(3));
	mvwprintw(stat_win,2,1,"Use Arrow Key to control move");
	mvwprintw(stat_win,3,1,"Press Q to quit");
	init_pair(4,COLOR_MAGENTA,COLOR_BLACK);
	wattron(stat_win,COLOR_PAIR(4));
	mvwprintw(stat_win,4,1,"Score %6d",0);
	//wattroff(stat_win,COLOR_PAIR);
	wrefresh(stat_win);
	generate_new(board);
	show_game(game_win, board,gap_row,gap_col);
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


#include "game.h"
using namespace std;
int Game::h_score=0;
int Game::mark=4;

Game::Game() {
    board=vector<vector<int> >(4, vector<int> (4,0));
    //2 random cells initialized with 2 and 2/4.
    unsigned int pos1,pos2;
    do {
        pos1=rand()%16;
        pos2=rand()%16;
    } while (pos1==pos2);
    for (unsigned int i=0;i<board.size();i++)
        for (unsigned int j=0;j<board[i].size();j++)
            if (i*board.size()+j==pos1)
                board[i][j]=2;
            else if (i*board.size()+j==pos2)
                board[i][j]=rand()%2?2:4;
            else board[i][j]=0;
    score=0;
    scorechanged=true;
    gamechanged=true;
    markchanged=false;
}

Game:: ~Game() {
}

bool Game::isGameover() {
    for (unsigned int i=0;i<board.size();i++) {
        for (unsigned int j=0;j<board[i].size();j++) {
            if (!board[i][j]) return false;
            //compare with next row
            if (i+1<board.size() && abs(board[i][j])==abs(board[i+1][j])) return false;
            if (j+1<board[i].size() && abs(board[i][j])==abs(board[i][j+1])) return false;	}
    }
    return true;
}

void Game::move_left() {
    //merge and  move cells, use negative number to record merged cells
    int row=board.size();
    int col=board[0].size();
    int zero_count=0;
    gamechanged=false;
    scorechanged=false;
    hscorechanged=false;
    markchanged=false;


    int newscore=0;
    for (int i=0;i<row;i++)
        for (int j=0;j<col;j++)
            if (board[i][j]<0)
                board[i][j]=-board[i][j]; //make all values positive to clear state for merge cell record
    //combine the same number and set the cell to right as 0
    for (int i=0;i<row;i++)
        for (int j=0;j<col-1;j++) {
            if (board[i][j]) { //skip zeros
                for (int k=j+1;k<col;k++) {
                    if (board[i][k]!=0) {
                        if (board[i][j]==board[i][k]) {
                            gamechanged=true;
                            newscore+=board[i][j];
                            board[i][j]=-(board[i][j]<<1); //record change position by negative
                            if (abs(board[i][j])>= mark) {
                                mark=abs(board[i][j])<<1; //update highest cell achived after merge
                                markchanged=true;}
                            board[i][k]=0;
                            j=k; }
                        break; }
                }
            }
        }
    //shift left if 0 to its left
    for (int i=0;i<row;i++) {
        int cur=0;
        for (int j=0;j<col;j++) {
            if (board[i][j]!=0) {
                if (j!=cur) {
                    gamechanged=true;
                    board[i][cur]=board[i][j];
                    board[i][j]=0;}
                cur++;}
        }
        zero_count=zero_count+board.size()-cur;
    }
    if (gamechanged) {//if any move then generate 2 at random pos
        int pos=rand()%zero_count+1;
        int count=0;
        for (unsigned int i=0;i<board.size();i++)
            for (unsigned int j=0;j<board[i].size();j++) {
                if (!board[i][j]) {
                    count++;
                    if (count==pos) board[i][j]=2;}
            }
    }
    scorechanged=newscore!=0;
    score+=newscore;
    if (score>=h_score) {
        h_score=score;
        hscorechanged=true;}
}
void Game::rotate_left () {
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
void Game::rotate_right () {
    rotate_left();
    rotate_left();
    rotate_left();}
void Game::move(direction arrow) {
    switch(arrow) {
    case up:
        rotate_left();
        move_left();
        rotate_right();
        break;
    case down:
        rotate_right();
        move_left();
        rotate_left();
        break;
    case left:
        move_left();
        break;
    case right:
        rotate_left();
        rotate_left();
        move_left();
        rotate_right();
        rotate_right();
        break;
    default:
        break;
    }
}
vector<vector<int> > Game::getBoard() {
    return board;}
int Game::getScore() {return score;}
int Game::getHScore() {return h_score;}
int Game::getMark() {return mark;}
bool Game::isMarkChanged() {return markchanged;}
bool Game::isGameChanged() { return gamechanged;}
bool Game::isScoreChanged() { return  scorechanged;}
bool Game::isHScoreChanged() { return  hscorechanged;}
void Game::setHScore(int num) {h_score=num;}
void Game::setMark(int num) {mark=num;}


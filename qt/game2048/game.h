#ifndef GAME
#define GAME
#include <vector>
#include <stdlib.h>
class Game {
public:
    typedef enum direction {up,down,left,right} direction;
    Game();
    ~Game();
    bool isGameover();
    void newGame();
    bool isGameChanged();
    bool isScoreChanged();
    bool isMarkChanged();
    void move(direction arrow); //move to a direction, return if game changed or not
    std::vector<std::vector<int> > getBoard();
    int getScore();
    int getHScore();
    int getMark();
private:
    int score; //current score for the game
    bool scorechanged; //indicate if there's any cell merged
    bool gamechanged; //indicate if game has changed, including move
    bool markchanged; //indicate if highest tile changed
    static int mark; //highest merged mark
    static int h_score; //highest score
    std::vector<std::vector<int> > board; //the board matrix
    void move_left();//returning if game changed or not
    void rotate_left(); //rotate the board, helper for move
    void rotate_right(); //helper for rotating board based on rotate_left
};

#endif // GAME



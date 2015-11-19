#ifndef GAME
#define GAME
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
class Game {
public:

    typedef enum direction {up,down,left,right} direction;
    Game();
    ~Game();
    bool isGameover();
    void newGame();
    bool isGameChanged();
    bool isScoreChanged();
     bool isHScoreChanged();
    bool isMarkChanged();
    void move(direction arrow); //move to a direction, return if game changed or not
    std::vector<std::vector<int> > getBoard();
    int getScore();
    int getHScore();
    int getMark();
    void setHScore(int);
    void setMark(int);
private:
    int score; //current score for the game
    bool scorechanged; //indicate if there's any cell merged
    bool hscorechanged; //indicate if highest score changed
    bool gamechanged; //indicate if game has changed, including move
    bool markchanged; //indicate if highest tile changed
    static int mark; //highest merged mark
    static int h_score; //highest score
    std::vector<std::vector<int> > board; //the board matrix
    void move_left();//returning if game changed or not
    void rotate_left(); //rotate the board, helper for move
    void rotate_right(); //helper for rotating board based on rotate_left

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive & ar, const unsigned version ) {
        ar & score;
        ar & h_score;
        ar & mark;
        ar & board;
    }

};


#endif // GAME



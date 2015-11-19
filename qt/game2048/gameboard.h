#ifndef GAMEBOARD
#define GAMEBOARD
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QtGlobal>
#include <QKeyEvent>
#include <QDialog>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QLinearGradient>
#include "game.h"
#include <QDebug>

class gameBoard: public QWidget {
    Q_OBJECT
public:
    explicit gameBoard(QWidget* parent=0);
    ~gameBoard();
    void paint();
    void create_Menu();
    void readGame();
    void closeEvent (QCloseEvent * event );
protected:
    void keyPressEvent(QKeyEvent* event);
signals:
    void isGameOver();
    void isGameWin();
        void closed();
public slots:
    void menu_Clicked();
    void cont_Clicked();
    void restart_Clicked();
    void restartGame_Clicked();
    void update();
    void showGameOver();
     void showGameWin();
    void saveGame();
    void resetGame();

private:
    Game* game; // the c++ game mechanics
    QGridLayout* gamelayout; //layout for the board
    QVBoxLayout* mainlayout; //layout for the app
    QWidget* board; //the actual game board
    QWidget* statuswin; //status, including menu
    QLabel* score; //current score
    QLabel* hscore; //highest score
    QLabel* statement; //statement for highest mark achievement
    QLabel* drawLabel(int num); //draw the actual tile
    QDialog* menu_log; //menu dialog
    QDialog* gameover; //gameover dialog
    QDialog* gamewin; //mark achievement dialog
    QPushButton* restart_game;//restart after game over
    QPushButton* reset_game; //reset all highscores and marks
    QPushButton* menu; //the menu button
    QPushButton* stats; //the stats button
    QPushButton* cont; //continue button
    QPushButton* win_cont; //continue in game win window
    QPushButton* restart; //restart button
    void decButton(QPushButton*); //the common decoration of button
};

#endif // GAMEBOARD



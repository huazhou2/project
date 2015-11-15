#include "gameboard.h"
#include <QApplication>
int main(int argc, char** argv) {
    QApplication app(argc,argv);

    gameBoard mygame;

    mygame.show();
    return app.exec();
}


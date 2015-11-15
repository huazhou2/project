#include "gameboard.h"

gameBoard::gameBoard(QWidget* parent): QWidget(parent) {
    //start new game and paint the app
    game=new Game();
    paint();
    create_Menu();
    connect(menu,SIGNAL(clicked()),this,SLOT(menu_Clicked()));
    connect(cont,SIGNAL(clicked()),this, SLOT(cont_Clicked()));
    connect(restart,SIGNAL(clicked()),this, SLOT(restart_Clicked()));
    connect(this,SIGNAL(isGameOver()),this,SLOT(showGameOver()));
    connect(restart_game,SIGNAL(clicked()),this,SLOT(restartGame_Clicked()));
}
gameBoard::~gameBoard() {

}
void gameBoard::paint() {
    QHBoxLayout* toplayer=new QHBoxLayout();

    statuswin=new QWidget(this);
    QGridLayout* status_lay=new QGridLayout(statuswin);
    QLabel *gametitle=new QLabel("2048");
    gametitle->setStyleSheet("background-color: orange;"
                             "color: white;"
                             "font: arial;"
                             "font-size: 50px;"
                             "border-radius: 10px;"
                             "qproperty-alignment: AlignCenter;");
    gametitle->setFixedSize(120,120);
    gametitle->setFrameStyle(2);
    score=new QLabel(QString("Current Score\n %1").arg(game->getScore()));
    score->setStyleSheet("background-color: grey;"
                         "color: white;"
                         "font: arial;"
                         "font-size: 22px;"
                         "border-radius: 10px;"
                         "border: 2px solid cyan;"
                         "qproperty-alignment: AlignCenter;");
    score->setFixedSize(150,70);

    hscore=new QLabel(QString("Highest Score\n %1").arg(game->getHScore()));
    hscore->setStyleSheet("background-color: grey;"
                          "color: white;"
                          "font: arial;"
                          "font-size: 22px;"
                          "border-radius: 10px;"
                          "border: 2px solid cyan;"
                          "qproperty-alignment: AlignCenter;");
    hscore->setFixedSize(150,70);
    menu=new QPushButton("Menu");
    stats=new QPushButton("Stats");
    stats->setFocusPolicy(Qt::NoFocus); //dont capture keypress
    menu->setFocusPolicy(Qt::NoFocus);
    decButton(stats);
    decButton(menu);

    menu->setFixedSize(120,40);
    stats->setFixedSize(120,40);

    status_lay->addWidget(score,0,0,2,1,Qt::AlignCenter);
    status_lay->addWidget(hscore,0,1,2,1,Qt::AlignCenter);
    status_lay->addWidget(menu,2,0,Qt::AlignCenter);
    status_lay->addWidget(stats,2,1,Qt::AlignCenter);
    //set spacing among menu,status,score and hscore
    status_lay->setHorizontalSpacing(10);
    status_lay->setVerticalSpacing(10);

    toplayer->addWidget(gametitle,5);
    toplayer->addWidget(statuswin,5);

    mainlayout=new QVBoxLayout(this);
    QHBoxLayout* hlayout=new QHBoxLayout();

    hlayout->setAlignment(hscore,Qt::AlignCenter);
    board=new QWidget(this);

    board->setStyleSheet("background-color: rgb(96,96,96);");

    toplayer->insertWidget(1,board);
    gamelayout=new QGridLayout();
    board->setLayout(gamelayout);

    std::vector<std::vector<int> > curgame=game->getBoard();
    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            QLabel* tile=drawLabel(curgame[i][j]);
            gamelayout->addWidget(tile,i,j);
        }
    }
    QSizePolicy qsp(QSizePolicy::Preferred,QSizePolicy::Preferred);
    qsp.setHeightForWidth(true);
    board->setSizePolicy(qsp);
    statement=new QLabel(QString("You new challenge is to get %1").arg(game->getMark()));
    statement->setStyleSheet(
                "color: black;"
                "font: arial;"
                "font-size: 14px;"
                "border-radius: 10px;");
    mainlayout->insertLayout(1,toplayer,4);
    mainlayout->insertWidget(2,statement,1);
    mainlayout->insertWidget(3,board,16);
    setFixedSize(460*1,650*1);
    setWindowTitle("Hua's 2048");
}

void gameBoard::update() {  
    delete board;
    board=new QWidget(this);
    board->setStyleSheet("background-color: rgb(96,96,96);");
    QGridLayout* board_lay=new QGridLayout(board);
    std::vector<std::vector<int> > curBoard=game->getBoard();
    for (int i=0;i<4;i++) {
        for (int j=0;j<4;j++) {
            QLabel* tile=drawLabel(curBoard[i][j]);
            if ( curBoard[i][j]<0) {//merged cell then animate the opacity
                QGraphicsOpacityEffect* effect=new QGraphicsOpacityEffect(tile);
                tile->setGraphicsEffect(effect);
                QPropertyAnimation* animation=new QPropertyAnimation(effect,"opacity");
                effect->setOpacity(0.4);
                animation->setDuration(100);
                animation->setKeyValueAt(0.0,0.4);
                animation->setKeyValueAt(1.0,1.0);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                }
            board_lay->addWidget(tile,i,j);
        }
    }
    mainlayout->insertWidget(3,board,16);
}

void gameBoard::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_Left:
        game->move(Game::left);
        break;
    case Qt::Key_Right:
        game->move(Game::right);
        break;
    case Qt::Key_Up:
        game->move(Game::up);
        break;
    case Qt::Key_Down:
        game->move(Game::down);
        break;
    }
    if (game->isGameChanged()) update();
    if (game->isScoreChanged()) {
        score->setText(QString("Current Score\n %1").arg(game->getScore()));
        //set highscore if cur score equals highest score
        if (game->getScore()==game->getHScore())
            hscore->setText(QString("Highest Score\n %1").arg(game->getHScore()));
        if (game->isMarkChanged()) statement->setText(QString("You new challenge is to get %1").arg(game->getMark()));
    }
    if (game->isGameover()) emit isGameOver();
}

QLabel* gameBoard::drawLabel(int num){ //setup color
    QLabel* tile=new QLabel();
    num=abs(num);
    switch (num) {
    case 0:
        tile->setStyleSheet("background-color: rgb(160,160,160);" );
        break;
    case 2:
        tile->setStyleSheet("background-color: rgb(224,224,224);"
                            "color: black;"
                            );
        tile->setText(QString::number(num));
        break;
    case 4:
        tile->setStyleSheet("background-color: rgb(218,218,191);"
                            "color: black;"
                            );
        tile->setText(QString::number(num));
        break;
    case 8:
        tile->setStyleSheet("background-color: rgb(255,178,102);"
                            "color: white;");
        tile->setText(QString::number(num));
        break;
    case 16:
        tile->setStyleSheet("background-color: rgb(245,128,0);"
                            "color: white;");
        tile->setText(QString::number(num));
        break;

    case 32:
        tile->setStyleSheet("background-color: rgb(255,102,102);"
                            "color: white;");
        tile->setText(QString::number(num));
        break;
    case 64:
        tile->setStyleSheet("background-color: rgb(245,73,0);"
                            "color: white;");
        tile->setText(QString::number(num));
        break;
    case 128:
        tile->setStyleSheet("background-color: rgb(255,255,102);"
                            "color: black;");
        tile->setText(QString::number(num));
        break;
    case 256:
        tile->setStyleSheet("background-color: rgb(255,255,131);"
                            "color: black;");
        tile->setText(QString::number(num));
        break;
    case 512:
        tile->setStyleSheet("background-color: rgb(255,255,0);"
                            "color: black;");
        tile->setText(QString::number(num));
        break;
    case 1024:
        tile->setStyleSheet("background-color: rgb(102,178,255);"
                            "color: white;");
        tile->setText(QString::number(num));
        break;
    case 2048:
        tile->setStyleSheet("background-color: rgb(51,51,255);"
                            "color: white;");
        tile->setText(QString::number(num));
        break;
    case 4092:
        tile->setStyleSheet("background-color: rgb(0,204,0);"
                            "color: white;");
        tile->setText(QString::number(num));
        break;
    default:
        tile->setStyleSheet("background-color: rgb(204,0,204);"
                            "color: white;");
        tile->setText(QString::number(num));
        break;
    }
    //add common style
    tile->setStyleSheet(tile->styleSheet()
                        + "font: bold;"
                        + "border-radius: 10px;"
                        +  "font: arial;"
                        + "font-size: 44px;"
                        +"qproperty-alignment: AlignCenter;" );
    tile->setFixedSize(100,100);
    return tile;
}

void gameBoard::create_Menu() {
    menu_log=new QDialog();
    menu_log->resize(450,300);
    menu_log->setModal(true);
    cont=new QPushButton("Continue");
    restart=new QPushButton("Restart");
    restart_game=new QPushButton("Restart Game");
    restart_game->setFixedSize(200,60);
    cont->setFixedSize(200,60);
    restart->setFixedSize(200,60);
    decButton(restart_game);
    decButton(cont);
    decButton(restart);
    QVBoxLayout* menu_lay=new QVBoxLayout(menu_log);
    menu_lay->addWidget(cont,0,Qt::AlignCenter);
    menu_lay->addWidget(restart,0,Qt::AlignCenter);
}
void gameBoard::showGameOver() {
    gameover=new QDialog();
    gameover->setStyleSheet("background-color: rgb(0,153,153)");
    gameover->resize(450,300);
    gameover->setModal(true);
    QLabel* gameover_lab=new QLabel("Game Over");
    gameover_lab->setStyleSheet(
                "color: red;"
                "font: arial;"
                "font-size: 64px;"
                "border-radius: 10px;"
                "qproperty-alignment: AlignCenter;");
    QVBoxLayout* gameover_lay=new QVBoxLayout(gameover);
    gameover_lay->addWidget(gameover_lab,10,Qt::AlignCenter);
    gameover_lay->addWidget(restart_game,1,Qt::AlignCenter);
    gameover->show();
}
void gameBoard::menu_Clicked() {
    menu_log->show();
}
void gameBoard::cont_Clicked() {
    menu_log->hide();
}
void gameBoard::restart_Clicked() {
    menu_log->hide();
    game=new Game();
    if (game->isScoreChanged()) score->setText(QString("Current Score\n %1").arg(game->getScore()));
    update();
}
void gameBoard::restartGame_Clicked() {
    gameover->hide();
    game=new Game();
    if (game->isScoreChanged()) score->setText(QString("Current Score\n %1").arg(game->getScore()));
    update();
}
void gameBoard::decButton(QPushButton* butt) {
    butt->setStyleSheet(
                "QPushButton {"
                "color: white;"
                "font: arial;"
                "font-size: 24px;"
                "border-radius: 10px;"
                "border: 2px solid black;"
                "padding: 5px;"
                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(255,128,0), stop: 1 rgb(153,76,0));"
                "}"
                "QPushButton:pressed {"
                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(0,0,204), stop: 1 rgb(204,229,255));"
                "}"
                "QPushButton:hover:!pressed {"
                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(0,153,76), stop: 1 rgb(204,255,204));"
                "}"
                );
}


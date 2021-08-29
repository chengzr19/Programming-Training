#include <QDir>
#include <QLayout>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Chess");

    m_pTimer = new QTimer(this);

    initSocket();

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::gray);
    ui->statusLabel->setPalette(pe);
    ui->colorLabel->setPalette(pe);
    ui->activeLabel->setPalette(pe);
    ui->totalLabel->setPalette(pe);
    ui->timeoutLabel->setPalette(pe);
    ui->timeleftLabel->setPalette(pe);
    ui->chessportLabel->setPalette(pe);

    initChess();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Basic communication functions
void MainWindow::sendMessage(QString s)
{
    if (isServer) server->sendMessage(s);
    else client->sendMessage(s);
}

void MainWindow::showMessage(QString title, QString content, int type)
{
    Thread * thread = new Thread(title, content, type);
    thread->start();
}

void MainWindow::connectSucc()
{
    isConnected = true;
    if (isServer) showMessage("", "You have connected to the client successfully!", 1);
    else showMessage("", "You have connected to the server successfully!", 1);
}

void MainWindow::connectFail()
{
    client->disconnect();
    showMessage("", "Connection timed out! Please check your input!", 2);
}

//Start the game
void MainWindow::gameStart()
{
    // qDebug() << isServer << " gamestart";
    count_down(interval);
    if (isServer)
    {
        turn = server->turn;
        QVector<int> index(60);
        for (int i = 0; i < 60; i++) index[i] = i;
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle (index.begin(), index.end(), std::default_random_engine(seed));
        int id = 0;
        for (int i = 0; i < 60; i++)
        {
            int nrow, ncol;
            nrow = index[i] / 5;
            ncol = index[i] % 5;
            if (!play.board[nrow][ncol].isCamp)
            {
                chess[id]->posi.x = nrow;
                chess[id]->posi.y = ncol;
                id++;
            }
        }
        QString str;
        for (int i = 0; i < 50; i++) str.append(QString::number(chess[i]->posi.x * 5 + chess[i]->posi.y) + "/");
        server->sendMessage(str);
    }
    else
    {
        turn = client->turn;
        for (int i = 0; i < 50; i++)
        {
            chess[i]->posi.x = client->index[i] / 5;
            chess[i]->posi.y = client->index[i] % 5;
        }
    }
    if (turn == 0) {
        ui->status->setText("ACT");
    }
    else {
        ui->status->setText("WAIT");
    }
    for (int i = 0; i < 50; i++)
    {
        QImage img;
        QString path = ":/Board/png/";
        img.load (path + "Chess.png");
        QPixmap pixmap(QPixmap::fromImage(img));
        chess[i]->setScaledContents(true);
        chess[i]->setPixmap(pixmap);
        chess[i]->setGeometry(play.board[chess[i]->posi.x][chess[i]->posi.y].x,play.board[chess[i]->posi.x][chess[i]->posi.y].y,50,25);
        play.board[chess[i]->posi.x][chess[i]->posi.y].id = i;
    }
    connect(this, &MainWindow::clicked, this, &MainWindow::chessClicked);
    connect(this, &MainWindow::win, this, &MainWindow::gameEnd);
    connect(m_pTimer, &QTimer::timeout, this, &MainWindow::timerTimeout);
    ui->totalStep->display(QString::number(total));
    ui->colorBrowser->setText("UNKOWN");
    ui->activePlayer->setText("ODD");
    isStarted = true;
}

void MainWindow::initSocket()
{
    server = new Server(this);
    connect(server, &Server::connectSucc, this, &MainWindow::connectSucc);
    connect(server, &Server::bothReady, this, &MainWindow::gameStart);
    connect(server, &Server::clicked, this, &MainWindow::clicked);
    connect(server, &Server::flip, this, &MainWindow::flip);
    connect(server, &Server::occupy, this, &MainWindow::occupy);
    connect(server, &Server::eat_succ, this, &MainWindow::eat_succ);
    connect(server, &Server::eat_draw, this, &MainWindow::eat_draw);
    connect(server, &Server::eat_fail, this, &MainWindow::eat_fail);
    connect(server, &Server::eat_flag, this, &MainWindow::eat_flag);
    connect(server, &Server::win, this, &MainWindow::gameEnd);

    client = new Client(this);
    connect(client, &Client::connectSucc, this, &MainWindow::connectSucc);
    connect(client, &Client::fail_connect, this, &MainWindow::connectFail);
    connect(client, &Client::bothReady, this, &MainWindow::gameStart);
    connect(client, &Client::clicked, this, &MainWindow::clicked);
    connect(client, &Client::flip, this, &MainWindow::flip);
    connect(client, &Client::occupy, this, &MainWindow::occupy);
    connect(client, &Client::eat_succ, this, &MainWindow::eat_succ);
    connect(client, &Client::eat_draw, this, &MainWindow::eat_draw);
    connect(client, &Client::eat_fail, this, &MainWindow::eat_fail);
    connect(client, &Client::eat_flag, this, &MainWindow::eat_flag);
    connect(client, &Client::win, this, &MainWindow::gameEnd);
}

void MainWindow::initChess()
{
    {
        chess[0] = ui->chess_0;
        chess[1] = ui->chess_1;
        chess[2] = ui->chess_2;
        chess[3] = ui->chess_3;
        chess[4] = ui->chess_4;
        chess[5] = ui->chess_5;
        chess[6] = ui->chess_6;
        chess[7] = ui->chess_7;
        chess[8] = ui->chess_8;
        chess[9] = ui->chess_9;
        chess[10] = ui->chess_10;
        chess[11] = ui->chess_11;
        chess[12] = ui->chess_12;
        chess[13] = ui->chess_13;
        chess[14] = ui->chess_14;
        chess[15] = ui->chess_15;
        chess[16] = ui->chess_16;
        chess[17] = ui->chess_17;
        chess[18] = ui->chess_18;
        chess[19] = ui->chess_19;
        chess[20] = ui->chess_20;
        chess[21] = ui->chess_21;
        chess[22] = ui->chess_22;
        chess[23] = ui->chess_23;
        chess[24] = ui->chess_24;
        chess[25] = ui->chess_25;
        chess[26] = ui->chess_26;
        chess[27] = ui->chess_27;
        chess[28] = ui->chess_28;
        chess[29] = ui->chess_29;
        chess[30] = ui->chess_30;
        chess[31] = ui->chess_31;
        chess[32] = ui->chess_32;
        chess[33] = ui->chess_33;
        chess[34] = ui->chess_34;
        chess[35] = ui->chess_35;
        chess[36] = ui->chess_36;
        chess[37] = ui->chess_37;
        chess[38] = ui->chess_38;
        chess[39] = ui->chess_39;
        chess[40] = ui->chess_40;
        chess[41] = ui->chess_41;
        chess[42] = ui->chess_42;
        chess[43] = ui->chess_43;
        chess[44] = ui->chess_44;
        chess[45] = ui->chess_45;
        chess[46] = ui->chess_46;
        chess[47] = ui->chess_47;
        chess[48] = ui->chess_48;
        chess[49] = ui->chess_49;
    }
    for (int i = 0; i < 50; i++) chess[i]->id = i;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 25; j++)
        {
            int id = i * 25 + j;
            switch (i) {
                case 0: chess[id]->color = 1; break;
                case 1: chess[id]->color = 2; break;
                default: chess[id]->color = 0;
            }
            switch (j) {
                case 0: chess[id]->type = 1; break;
                case 1: chess[id]->type = 2; break;
                case 2: chess[id]->type = 3; break;
                case 3: chess[id]->type = 3; break;
                case 4: chess[id]->type = 4; break;
                case 5: chess[id]->type = 4; break;
                case 6: chess[id]->type = 5; break;
                case 7: chess[id]->type = 5; break;
                case 8: chess[id]->type = 6; break;
                case 9: chess[id]->type = 6; break;
                case 10: chess[id]->type = 7; break;
                case 11: chess[id]->type = 7; break;
                case 12: chess[id]->type = 7; break;
                case 13: chess[id]->type = 8; break;
                case 14: chess[id]->type = 8; break;
                case 15: chess[id]->type = 8; break;
                case 16: chess[id]->type = 9; break;
                case 17: chess[id]->type = 9; break;
                case 18: chess[id]->type = 9; break;
                case 19: chess[id]->type = 10; break;
                case 20: chess[id]->type = 11; break;
                case 21: chess[id]->type = 11; break;
                case 22: chess[id]->type = 12; break;
                case 23: chess[id]->type = 12; break;
                case 24: chess[id]->type = 12; break;
            }
        }
    for (int i = 0; i < 50; i++)
    {
        QImage img;
        QString path = ":/Board/png/";
        img.load (path + "Chess.png");
        QPixmap pixmap(QPixmap::fromImage(img));
        chess[i]->setScaledContents(true);
        chess[i]->setPixmap(pixmap);
        if (chess[i]->color == 1) chess[i]->setGeometry(410, 480 - i * 5, 50, 25);
        if (chess[i]->color == 2) chess[i]->setGeometry(470, 480 - (i - 25) * 5, 50, 25);
        chess[i]->raise();
    }
}

//End the game
void MainWindow::gameEnd(int color)
{
    if (!isEnd)
    {
        isEnd = true;
        m_pTimer->stop();
        if (color == 1 && side == 1) showMessage("", "Congratulations! RED has won the game!", 1);
        if (color == 1 && side == 2) showMessage("", "Sorry! BLUE has lost the game!", 1);
        if (color == 2 && side == 1) showMessage("", "Sorry! RED has lost the game!", 1);
        if (color == 2 && side == 2) showMessage("", "Congratulations! BLUE has won the game!", 1);
        if (color == 3) showMessage("", "Congratulations! You have won the game!", 1);
        if (color == 0) showMessage("", "Sorry! You have lost the game!", 1);
    }
    for (int i = 0; i < 50; i++)
    {
        QImage img;
        QString path = ":/Board/png/";
        img.load (path + "Chess.png");
        QPixmap pixmap(QPixmap::fromImage(img));
        chess[i]->setScaledContents(true);
        chess[i]->setPixmap(pixmap);
        if (chess[i]->color == 1) chess[i]->setGeometry(410, 480 - i * 5, 50, 25);
        if (chess[i]->color == 2) chess[i]->setGeometry(470, 480 - (i - 25) * 5, 50, 25);
        chess[i]->raise();
        play.board[chess[i]->posi.x][chess[i]->posi.y].id = -1;
    }
    ui->totalStep->display("");
    ui->timeLeft->display("");
    ui->timeoutTimes->display("");
    ui->activePlayer->setText("");
    ui->colorBrowser->setText("");
    ui->status->setText("");
}

//Click on chesses
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint point = event->pos() - centralWidget()->pos();
    // qDebug() << point << event->pos() << centralWidget()->pos();
    if (total % 2 == turn)
    {
        if (event->button() == Qt::LeftButton){
            int a = point.x(), b = point.y();
            int ncol,nrow;
            if (a >= 20 && a <= 380) ncol = 2 * (a - 20) / 155;
            else ncol = -1;
            if (b <= 230 && b >= 20 ) nrow = (b - 20) / 37;
            else if (b >= 320 && b <=530) nrow = 6 + (b - 320) / 37;
            else nrow = -1;
            if (nrow != -1 && ncol != -1) emit clicked(nrow, ncol);
            else showMessage("", "Invalid Click!", 2);
        }
    }
    else
    {
        showMessage("", "It isn't your turn to act now!", 2);
    }
}

void MainWindow::chessClicked(int x, int y)
{
    if (carry == -1)
    {
        if (play.board[x][y].id == -1) showMessage("", "There is no chess here!", 2);
        else
        {
            if (!chess[play.board[x][y].id]->isFlipped) { flip(x,y); sendMessage("Flip/" + QString::number(x) + "/" + QString::number(y) + "/"); }
            else
            {
                if ((current != 0 && chess[play.board[x][y].id]->color == current) || current == 0)
                {
                    if (!chess[play.board[x][y].id]->isChosen)
                    {
                        if (chess[play.board[x][y].id]->isMovable())
                        {
                            chess[play.board[x][y].id]->isChosen = true;
                            carry = play.board[x][y].id;
                        }
                        else
                        {
                            showMessage("", "This chess cannot be moved!", 2);
                        }
                    }
                }
                else
                {
                    if (current == 1) showMessage("", "You cannot move BLUE chesses!", 2);
                    if (current == 2) showMessage("", "You cannot move RED chesses!", 2);
                }
            }
        }
    }
    else
    {
        chess[carry]->isChosen = false;
        on_chess_move(chess[carry]->posi.x, chess[carry]->posi.y, x, y);
        carry = -1;
    }
}

//Decide the color
void MainWindow::decideColor()
{
    if (evenblue == 2) { even = 2; odd = 1; current = 1; }
    if (evenred == 2) { even = 1; odd = 2; current = 2; }
    if (oddblue == 2) { even = 1; odd = 2; current = 1; }
    if (oddred == 2) { even = 2; odd = 1; current = 2; }
    if (odd == 1 && even ==2)
    {
        if (turn == 0) { side = 1; ui->colorBrowser->setText("RED"); }
        else { side = 2; ui->colorBrowser->setText("BLUE"); }
    }
    if (odd == 2 && even ==1)
    {
        if (turn == 0) { side = 2; ui->colorBrowser->setText("BLUE"); }
        else { side = 1; ui->colorBrowser->setText("RED"); }
    }
}

//Eat chesses
void MainWindow::on_chess_move(int x1, int y1, int x2, int y2)
{
    int guest = play.board[x1][y1].id;
    int host = play.board[x2][y2].id;
    QString path = QString::number(x1) + "/" + QString::number(y1) + "/" + QString::number(x2) + "/" + QString::number(y2) + "/";
    if (play.isValid(chess[guest]->type, x1, y1, x2, y2)){
        if (host == -1) { occupy(x1, y1, x2, y2); sendMessage("Occupy/" + path); return;}
        else
        {
            if (play.board[x2][y2].isCamp) { showMessage("", "Chesses in the camp cannot be eaten!", 2); return;}
            else //Host position is not camp.
            {
                if (!chess[host]->isFlipped)
                { showMessage("", "This chess hasn't been flipped!", 2); return; }
                else //Host has been flipped.
                {
                    if (chess[host]->color != chess[guest]->color)
                    {
                        int result = 0;
                        if (guest >= 0 && host >= 0) result = play.isEatable(chess[guest]->type, chess[host]->type);
                        if (result == 1) { eat_succ(x1, y1, x2, y2); sendMessage("Succ/" + path); return; }
                        else if (result == 0) { eat_draw(x1, y1, x2, y2); sendMessage("Draw/" + path); return; }
                        // else if (result == -1) { eat_fail(x1, y1, x2, y2); sendMessage("Fail/" + path); return; }
                        else
                        {
                            if (chess[host]->type == 10)
                            {
                                if ((chess[guest]->color == 2 && count_mine(1) == 3) || (chess[guest]->color == 1 && count_mine(2) == 3))
                                {
                                    eat_flag(x1, y1, x2, y2);
                                    sendMessage("Flag/" + path + QString::number(chess[guest]->color) + "/");
                                    emit win(chess[guest]->color);
                                    return;
                                }
                                else { showMessage("", "The military flag cannot be moved now!", 2); return; }
                            }
                            else { showMessage("", "This chess cannot be eaten now!", 2); return; }
                        }
                    }
                    else
                    {
                        showMessage("", "You cannot eat chesses of your side!", 2); return;
                    }
                }
            }
        }
    }
    else { showMessage("", "Please move the chess to the right position!", 2); return; }
}

void MainWindow::flip(int x, int y)
{
    QImage img;
    QString path = ":/Chess/png/";
    QString stype = QString::number(chess[play.board[x][y].id]->type);
    QString scolor = "";
    if (chess[play.board[x][y].id]->color == 1) scolor = "R";
    else if (chess[play.board[x][y].id]->color == 2) scolor = "B";
    img.load (path + stype + scolor + ".png");
    QPixmap pixmap(QPixmap::fromImage(img));
    chess[play.board[x][y].id]->setScaledContents(true);
    chess[play.board[x][y].id]->setPixmap(pixmap);
    chess[play.board[x][y].id]->isFlipped = true;
    chess[play.board[x][y].id]->isChosen = false;
    on_finish(chess[play.board[x][y].id]->color);
}

void MainWindow::occupy(int x1, int y1, int x2, int y2)
{
    int guest = play.board[x1][y1].id;
    play.board[x1][y1].id = -1;
    play.board[x2][y2].id = guest;
    chess[guest]->posi.x = x2;
    chess[guest]->posi.y = y2;
    chess[guest]->setGeometry(play.board[x2][y2].x, play.board[x2][y2].y, 50, 25);
    on_finish(chess[guest]->color);
}

void MainWindow::eat_succ(int x1, int y1, int x2, int y2)
{
    int guest = play.board[x1][y1].id;
    int host = play.board[x2][y2].id;
    chess[host]->die();
    play.board[x2][y2].id = guest;
    play.board[x1][y1].id = -1;
    chess[guest]->posi.x = x2;
    chess[guest]->posi.y = y2;
    chess[guest]->setGeometry(play.board[x2][y2].x, play.board[x2][y2].y, 50, 25);
    on_finish(chess[guest]->color);
}

void MainWindow::eat_draw(int x1, int y1, int x2, int y2)
{
    int guest = play.board[x1][y1].id;
    int host = play.board[x2][y2].id;
    chess[host]->die();
    chess[guest]->die();
    play.board[x1][y1].id = -1;
    play.board[x2][y2].id = -1;
    on_finish(chess[guest]->color);
}

void MainWindow::eat_fail(int x1, int y1, int x2, int y2)
{
    int guest = play.board[x1][y1].id;
    chess[guest]->die();
    play.board[x1][y1].id = -1;
    play.board[x2][y2].id = play.board[x2][y2].id;
    on_finish(chess[guest]->color);
}

void MainWindow::eat_flag(int x1, int y1, int x2, int y2)
{
    int guest = play.board[x1][y1].id;
    int host = play.board[x2][y2].id;
    chess[host]->die();
    play.board[x1][y1].id = -1;
    play.board[x2][y2].id = guest;
    chess[guest]->posi.x = x2;
    chess[guest]->posi.y = y2;
    chess[guest]->setGeometry(play.board[x2][y2].x, play.board[x2][y2].y, 50, 25);
    on_finish(chess[guest]->color);
}

int MainWindow::count_mine(int color)
{
    int result = 0;
    for (int i = 0; i < 50; i++)
    {
        if (chess[i]->isDead && chess[i]->type == 12 && chess[i]->color == color) result++;
    }
    return result;
}

//Update the information
void MainWindow::on_finish()
{
    total++;
    ui->totalStep->display(QString::number(total));
    if (current != 0) current = 3 - current;
    if (current == 1) ui->activePlayer->setText("RED");
    else if (current == 2) ui->activePlayer->setText("BLUE");
    else if (total % 2 == 0) ui->activePlayer->setText("ODD");
    else ui->activePlayer->setText("EVEN");
    if (ui->status->toPlainText() == "ACT") ui->status->setText("WAIT");
    else if (ui->status->toPlainText() == "WAIT") ui->status->setText("ACT");
    if (!isEnd) count_down(interval);
}

void MainWindow::on_finish(int color)
{
    total++;
    ui->totalStep->display(QString::number(total));
    if (total % 2 != 0) { if (color == 1) oddred++; if (color == 2) oddblue++; }
    else { if (color == 1) evenred++; if (color == 2) evenblue++; }
    if (current == 0) decideColor();
    else current = 3 - current;
    if (current == 1) ui->activePlayer->setText("RED");
    else if (current == 2) ui->activePlayer->setText("BLUE");
    else if (total % 2 == 0) ui->activePlayer->setText("ODD");
    else ui->activePlayer->setText("EVEN");
    if (ui->status->toPlainText() == "ACT") ui->status->setText("WAIT");
    else if (ui->status->toPlainText() == "WAIT") ui->status->setText("ACT");
    if (!isEnd) count_down(interval);
    if (color != 0)
    {
        if (!check_movable(color)) {
            sendMessage("Win/" + QString::number(3 - color) + "/");
            emit win(3 - color);
        }
        if (!check_movable(3 - color)) {
            sendMessage("Win/" + QString::number(color) + "/");
            emit win(color);
        }
    }
}

bool MainWindow::check_movable(int color)
{
    bool movable = false;
    int id, x1, y1, x2, y2;
    for (int i = 0; i < 25; i++)
    {
        id = (color - 1) * 25 + i;
        x1 = chess[id]->posi.x;
        y1 = chess[id]->posi.y;
        if (chess[id]->isDead || !chess[id]->isMovable()) continue;
        else {
            if (!chess[id]->isFlipped) return true;
            else
            {
                for (int j = 0; j < 60; j++)
                {
                    x2 = j / 5;
                    y2 = j % 5;
                    if (check_chess_move(x1, y1, x2, y2)) return true;
                }
            }
        }
    }
    return movable;
}

bool MainWindow::check_chess_move(int x1, int y1, int x2, int y2)
{
    int guest = play.board[x1][y1].id;
    if (guest == -1) return false;
    int host = play.board[x2][y2].id;
    if (play.isValid(chess[guest]->type, x1, y1, x2, y2)){
        if (host == -1) return true;
        else
        { //host != -1
            if (play.board[x2][y2].isCamp) return false;
            else //Host position is not camp.
            {
                if (!chess[host]->isFlipped) return true;
                else //Host has been flipped.
                {
                    if (chess[host]->color != chess[guest]->color)
                    {
                        int result = 0;
                        if (guest >= 0 && host >= 0) result = play.isEatable(chess[guest]->type, chess[host]->type);
                        if (result == 1 || result == 0) return true;
                        else
                        {
                            if (chess[host]->type == 10)
                            {
                                if ((chess[guest]->color == 2 && count_mine(1) == 3)
                                        ||  (chess[guest]->color == 1 && count_mine(2) == 3))
                                    return true;
                                else return false;
                            }
                            else return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }
    else return false;
}

//Count down and deal with timeout situations
void MainWindow::count_down(int startvalue)
{
    m_pTimer->stop();
    m_nStartValue = startvalue;
    m_pTimer->start(1000);
    timerTimeout();
}

void MainWindow::timerTimeout()
{
    if (m_nStartValue == 0)
    {
        m_pTimer->stop();
        ui->timeLeft->display(QString::number(0));
        if (total % 2 == turn)
        {
            timeout++;
            ui->timeoutTimes->display(QString::number(timeout));
            if (timeout == 3) {
                if (side != 0) { sendMessage("Win/" + QString::number(3 - side) + "/"); emit win(3 - side); return;}
                else { sendMessage("Win/" + QString::number(3) + "/"); emit win(0); return;}
            }
        }
        on_finish();
        return;
    }
    ui->timeLeft->display(QString::number(m_nStartValue));
    m_nStartValue--;
}

//Menubar operations
void MainWindow::on_actionStart_triggered()
{
    QMessageBox::StandardButton res
            = QMessageBox::question(NULL, "", "Do you want start a new game?", QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
    if (res == QMessageBox::Yes)
    {
        if (isServer) server->setReady();
        else client->sendMessage("Ready/");
    }
}

void MainWindow::on_actionSurrender_triggered()
{
    if (total >= 20) { sendMessage("Win/" + QString::number(3 - current) + "/"); emit win(3 - current);}
    else showMessage("Surrender", "You cannot surrender now!", 2);
}

void MainWindow::on_actionCreate_Connection_triggered()
{
    QMessageBox:: StandardButton result =
            QMessageBox::information(NULL, "Create Connection", "Host: " + server->addr.toString(), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
    if (result == QMessageBox::Ok)
    {
        isServer = true;
        server->beginListen();
        this->setWindowTitle("Chess Server");
    }
}

void MainWindow::on_actionConnect_to_Server_triggered()
{
    isServer = false;
    Connect * dialog = new Connect(this);
    connect(dialog, &Connect::address, client, &Client::connectServer);
    dialog->exec();
    this->setWindowTitle("Chess Client");
}

void MainWindow::on_actionCancel_Connection_triggered()
{
    if (isConnected)
    {
        isConnected = false;
        if (isStarted) {
            if (side != 0) { sendMessage("Win/" + QString::number(3 - side) + "/"); emit win(3 - side); return;}
            else { sendMessage("Win/" + QString::number(3) + "/"); emit win(0); return;}
        }
        if (isServer) server->disconnect();
        else client->disconnect();
    }
    else
    {
        showMessage("", "You haven't connected to the server or client so far!", 2);
    }
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    QMessageBox::StandardButton button;
        button=QMessageBox::question(this,"Exit", "Do you want to exit from the game?", QMessageBox::Yes|QMessageBox::No);
        if(button==QMessageBox::No)
        {
            event->ignore();
        }
        else if(button==QMessageBox::Yes)
        {
            if (!isEnd && isStarted)
            {
                if (side != 0) { sendMessage("Win/" + QString::number(3 - side) + "/"); emit win(3 - side);}
                else { sendMessage("Win/" + QString::number(3) + "/"); emit win(0);}
            }
            event->accept();
        }
}


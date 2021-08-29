#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include <QThread>
#include <QtNetwork>
#include <QCloseEvent>
#include "chess.h"
#include "board.h"
#include "client.h"
#include "server.h"
#include "connect.h"
#include "thread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Board play;
    QVector<Chess*> chess = QVector<Chess *>(50);
    Server* server;
    Client* client;
    QTimer* m_pTimer;
    int carry = -1;
    int current = 0, turn = 0, side = 0;
    int total = 0, oddred = 0, oddblue = 0, evenred = 0, evenblue = 0, odd = 0, even = 0;
    int m_nStartValue = 0, interval = 20, timeout = 0;
    bool isStarted = false, isEnd = false;
    bool isServer = true, isConnected = false;

    void sendMessage(QString s);
    void showMessage(QString title, QString content, int type);

    void decideColor();
    void decideOffensive();

    void mousePressEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

    void on_chess_move(int x1, int y1, int x2, int y2);
    bool check_movable(int color);
    bool check_chess_move(int x1, int y1, int x2, int y2);

    void flip(int x, int y);
    void occupy(int x1, int y1, int x2, int y2);
    void eat_succ(int x1, int y1, int x2, int y2);
    void eat_draw(int x1, int y1, int x2, int y2);
    void eat_fail(int x1, int y1, int x2, int y2);
    void eat_flag(int x1, int y1, int x2, int y2);
    int count_mine(int color);

    void count_down(int startvalue);

signals:
    void clicked(int, int);
    void win(int);

public slots:
    void gameStart();
    void gameEnd(int);
    void chessClicked(int, int);
    void on_finish();
    void on_finish(int);
    void timerTimeout();

private:
    Ui::MainWindow *ui;
    void initSocket();
    void initChess();

private slots:
    void connectSucc();
    void connectFail();
    void on_actionSurrender_triggered();
    void on_actionStart_triggered();
    void on_actionCreate_Connection_triggered();
    void on_actionConnect_to_Server_triggered();
    void on_actionCancel_Connection_triggered();

};
#endif // MAINWINDOW_H

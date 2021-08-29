#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QTime>
#include <QWidget>
#include <QStringList>
#include <QMessageBox>
#include <algorithm>
#include <random>
#include <chrono>

class Server : public QWidget
{
    Q_OBJECT
public:
    Server(QWidget *parent = nullptr);
    ~Server(){};
    int readyNum = 0;
    int turn = 0;
    bool hasListened = false;
    int randomNum(int m);
    void beginListen();
    void decideOffensive();
    void setReady();
    void disconnect();
    QHostAddress addr;
signals:
    void connectSucc();
    void bothReady();
    void clicked(int, int);
    void flip(int, int);
    void occupy(int, int, int, int);
    void eat_succ(int, int, int, int);
    void eat_draw(int, int, int, int);
    void eat_fail(int, int, int, int);
    void eat_flag(int, int, int, int);
    void win(int);

public slots:
    void socketRead();
    void sendMessage(QString);

private:
    QTcpServer *tcpserver;
    QTcpSocket *tcpsockets;

private slots:
    void newConnect();
};

#endif // SERVER_H

#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork>
#include <QWidget>
#include <QVector>
#include <QMessageBox>
#include <QTimer>

class Client : public QWidget
{
    Q_OBJECT
public:
    Client(QWidget *parent);
    ~Client(){};
    int turn = 0;
    void connectServer(QString);
    void disconnect();
    QVector<int> index;
    QTimer* pTimer;
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
    void fail_connect();
public slots:
    void socketRead();
    void sendMessage(QString);
    void connectFail();
private:
    QTcpSocket *tcpSocket;
};

#endif // CLIENT_H

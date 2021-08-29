#include "client.h"

Client::Client(QWidget *parent)
    :QWidget(parent)
{
    pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, this, &Client::connectFail);
}

void Client::connectServer(QString information){
    tcpSocket = new QTcpSocket;
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::socketRead);
    QHostAddress addr = QHostAddress(information);
    pTimer->start(10000);
    tcpSocket->connectToHost(QHostAddress(addr),8000);
}

void Client::socketRead(){
    QString strs;
    QStringList list;
    strs = tcpSocket->readAll();
    // qDebug() << "Client Reading" << strs;
    list = strs.split("/");
    if (list[0] == "BothReady")
    {
        index = QVector<int>(50);
        turn = list[1].toInt();
        for (int i = 0; i < 50; i++) index[i] = list[i + 2].toInt();
        emit bothReady();
    }
    if (list[0] == "Clicked") emit clicked(list[1].toInt(),list[2].toInt());
    if (list[0] == "Flip") emit flip(list[1].toInt(),list[2].toInt());
    if (list[0] == "Occupy") emit occupy(list[1].toInt(), list[2].toInt(), list[3].toInt(), list[4].toInt());
    if (list[0] == "Succ") emit eat_succ(list[1].toInt(), list[2].toInt(), list[3].toInt(), list[4].toInt());
    if (list[0] == "Draw") emit eat_draw(list[1].toInt(), list[2].toInt(), list[3].toInt(), list[4].toInt());
    if (list[0] == "Fail") emit eat_fail(list[1].toInt(), list[2].toInt(), list[3].toInt(), list[4].toInt());
    if (list[0] == "Flag")
    {
        emit eat_flag(list[1].toInt(), list[2].toInt(), list[3].toInt(), list[4].toInt());
        emit win(list[5].toInt());
    }
    if (list[0] == "Win") emit win(list[1].toInt());
    if (list[0] == "Connect")
    {
        pTimer->stop();
        emit connectSucc();
    }
}

void Client::sendMessage(QString s){
    // qDebug() << "Client Sending" << s;
    QByteArray sendMessage = s.toLocal8Bit();
    tcpSocket->write(sendMessage);
}

void Client::disconnect()
{
    if (tcpSocket!= nullptr) tcpSocket->disconnectFromHost();
}

void Client::connectFail()
{
    pTimer->stop();
    emit fail_connect();
}

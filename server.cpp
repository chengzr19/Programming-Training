#include "server.h"

Server::Server(QWidget *parent)
    : QWidget(parent)
{
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    foreach (QHostAddress address, info.addresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol){
            addr = address;
        }
    }
}

void Server::beginListen()
{
    if (!hasListened)
    {
        hasListened = true;
        tcpserver = new QTcpServer;
        if (!tcpserver->listen(addr, 8000)){
            qDebug() << tcpserver->errorString();
            close();
        }
        connect(tcpserver, &QTcpServer::newConnection, this, &Server::newConnect);
    }
}

void Server::newConnect(){
    tcpsockets = tcpserver->nextPendingConnection();
    connect(tcpsockets, &QTcpSocket::readyRead, this, &Server::socketRead);
    emit connectSucc();
    sendMessage("Connect/");
}


void Server::socketRead(){
    QString strs;
    if (tcpsockets->bytesAvailable() > 0) strs = tcpsockets->readAll();
    // qDebug() << "Server Reading" << strs;
    QStringList list = strs.split("/");
    if (list[0] == "Ready") setReady();
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
}

void Server::sendMessage(QString s){
    // qDebug() << "Server Sending" << s;
    QByteArray sendMessage = s.toLocal8Bit();
    tcpsockets->write(sendMessage);
}

void Server::decideOffensive(){
    QVector<int> index(2);
    for (int i = 0; i < 2; i++) index[i] = i;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (index.begin(), index.end(), std::default_random_engine(seed));
    turn = index[0];
    sendMessage(QString::number(index[1]) + "/");
}

void Server::setReady()
{
    readyNum++;
    if (readyNum == 2)
    {
        sendMessage("BothReady/");
        decideOffensive();
        emit bothReady();
    }
}

void Server::disconnect()
{
    if (tcpserver != nullptr) tcpserver->close();
    if (tcpsockets != nullptr) tcpsockets->disconnectFromHost();
}

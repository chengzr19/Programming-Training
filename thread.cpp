#include "thread.h"

void Thread::run()
{
    box->show(title, content, type);
}

void Box::readyShow()
{
    this->moveToThread(qApp->thread());
    QTimer::singleShot(0,this,SLOT(onShow()));
}

void Box::show(const QString &title, const QString &msg,const int type)
{
    QEventLoop eventLoop;
    auto messageBox = new Box(title,msg,type);
    connect(messageBox,SIGNAL(destroyed()),&eventLoop,SLOT(quit()));
    messageBox->readyShow();
    eventLoop.exec();
}

void Box::onShow()
{
    switch (type)
    {
    case 1:
        QMessageBox::information(NULL, title, msg, QMessageBox::Ok);
        break;
    case 2:
        QMessageBox::warning(NULL, title, msg, QMessageBox::Ok);
        break;
    case 3:
        QMessageBox::question(NULL, title, msg, QMessageBox::Yes | QMessageBox::No);
        break;
    }
    this->deleteLater();
}

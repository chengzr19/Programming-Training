#ifndef THREAD_H
#define THREAD_H

#include <QtGui>
#include <QtCore>
#include <QObject>
#include <QEventLoop>
#include <QMessageBox>
#include <QApplication>
#include <QTimer>

class Box : public QObject
{
    Q_OBJECT
public:
    explicit Box(QObject *parent = nullptr) : QObject(parent) {};
    void readyShow(void);
private:
    const QString title;
    const QString msg;
    int type;
public:
    Box(const QString &title, const QString &msg,const int type) : title(title),msg(msg),type(type) {};
    static void show(const QString &title,const QString &msg,const int type);
public slots:
    void onShow();
};

class Thread : public QThread
{
    Q_OBJECT
protected:
    void run();
public:
    Thread(QObject *parent=0) : QThread(parent){};
    Thread(const QString & title, const QString & content,const int type)
        :title(title), content(content), type(type)
    {
        box = new Box;
    }
    ~Thread(){};
    Box * box;
    QString title, content;
    int type;
signals:

public slots:

};

#endif // THREAD_H

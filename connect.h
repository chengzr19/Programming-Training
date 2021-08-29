#ifndef CONNECT_H
#define CONNECT_H

#include <QDialog>
#include <QHostAddress>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include "numberbutton.h"

namespace Ui {
class Connect;
}

class Connect : public QDialog
{
    Q_OBJECT

public:
    explicit Connect(QWidget *parent = nullptr);
    ~Connect();
    QVector<NumberButton *> button;
    QString info;
signals:
    void address(QString);
private slots:
    void input(int);
    void on_dotButton_clicked();
    void on_delButton_clicked();
    void on_cancelButton_clicked();
    void on_okButton_clicked();

private:
    Ui::Connect *ui;
};

#endif // CONNECT_H

#include "connect.h"
#include "ui_connect.h"
#include "numberbutton.h"

Connect::Connect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connect)
{
    ui->setupUi(this);
    button = QVector<NumberButton *>(10);

    button[0] = ui->Button0;
    button[1] = ui->Button1;
    button[2] = ui->Button2;
    button[3] = ui->Button3;
    button[4] = ui->Button4;
    button[5] = ui->Button5;
    button[6] = ui->Button6;
    button[7] = ui->Button7;
    button[8] = ui->Button8;
    button[9] = ui->Button9;

    for (int i = 0; i <= 9; i++)
    {
        button[i]->num = i;
        connect(button[i], SIGNAL(clicked(int)), this, SLOT(input(int)));
    }
    info = "";
}

Connect::~Connect()
{
    delete ui;
}

void Connect::input(int n)
{
    info += QString::number(n);
    // qDebug() << n << " " << info;
    ui->addressBrowser->setText(info);
}

void Connect::on_dotButton_clicked()
{
    info += ".";
    ui->addressBrowser->setText(info);
}


void Connect::on_delButton_clicked()
{
    info = info.mid(0, info.length()-1);
    ui->addressBrowser->setText(info);
}


void Connect::on_cancelButton_clicked()
{
    close();
}


void Connect::on_okButton_clicked()
{
    if (!QHostAddress(info).isNull()){
        // qDebug() << QHostAddress(info);
        QRegularExpression pattern("((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
        QRegularExpressionMatch check = pattern.match(info);
        if (check.hasMatch())
        {
            emit address(info);
            close();
        }
        else
        {
             QMessageBox::warning(NULL, "Warning", "Invalid address!");
        }
    }
    else
    {
        QMessageBox::warning(NULL, "Warning", "Invalid address!");
    }
}


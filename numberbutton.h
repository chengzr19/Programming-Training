#ifndef NUMBERBUTTON_H
#define NUMBERBUTTON_H

#include <QPushButton>

class NumberButton : public QPushButton
{
    Q_OBJECT
public:
    NumberButton(QWidget *parent = nullptr);
    ~NumberButton(){};
    int num;
signals:
    void clicked(int);
public slots:
    void numberClicked();
};

#endif // NUMBERBUTTON_H

#include "numberbutton.h"

NumberButton::NumberButton(QWidget *parent)
    :QPushButton(parent)
{
    connect(this, &QPushButton::clicked, this, &NumberButton::numberClicked);
}

void NumberButton::numberClicked()
{
    emit clicked(num);
}

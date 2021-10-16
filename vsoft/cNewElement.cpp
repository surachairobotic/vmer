#include "cNewElement.h"
#include "ui_cNewElement.h"

cNewElement::cNewElement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cNewElement)
{
    ui->setupUi(this);
}

cNewElement::~cNewElement()
{
    delete ui;
}

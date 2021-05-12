#include "cNewModelWindow.h"
#include "ui_cNewModelWindow.h"

cNewModelWindow::cNewModelWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cNewModelWindow)
{
    ui->setupUi(this);
}

cNewModelWindow::~cNewModelWindow() {
    delete ui;
}

void cNewModelWindow::createElementTable() {
    int row = db->elements.size();
    //ui->tableWidget = new QTableWidget(row, 1, this);
    ui->tableWidget->setRowCount(row);
    ui->tableWidget->setColumnCount(1);
    for(int i=0; i<row; i++) {
        QString txt = db->elements[i].name;
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(txt));
    }
}

void cNewModelWindow::on_pushButton_clicked()
{

}


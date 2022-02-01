#include "cNewShopWindow.h"
#include "ui_cNewShopWindow.h"
#include <QMessageBox>

cNewShopWindow::cNewShopWindow(QWidget *parent, int _plantId) :
    QDialog(parent),
    ui(new Ui::cNewShopWindow),
    plantId(_plantId)
{
    ui->setupUi(this);
}

cNewShopWindow::~cNewShopWindow() {
    delete ui;
}

void cNewShopWindow::createModelTable() {
    int row = db->models.size();
    ui->tableWidget_left->setRowCount(row);
    ui->tableWidget_left->setColumnCount(1);
    for(int i=0; i<row; i++) {
        QString txt = db->models[i].name;
        ui->tableWidget_left->setItem(i, 0, new QTableWidgetItem(txt));
    }
    ui->tableWidget_right->setColumnCount(1);
}

void cNewShopWindow::on_btn_add_clicked() {
    qDebug() << "on_btn_add_clicked";
    QList<QTableWidgetItem*> selectItem = ui->tableWidget_left->selectedItems();
    for(int i=0; i<selectItem.size(); i++) {
        qDebug() << selectItem[i]->text();
        int row = ui->tableWidget_right->rowCount();
        ui->tableWidget_right->setRowCount(row+1);
        ui->tableWidget_right->setItem(row, 0, new QTableWidgetItem(selectItem[i]->text()));
        int selRow = ui->tableWidget_left->row(selectItem[i]);
        //qDebug() << "selRow : " << selRow;
        model_ids.push_back(db->models[selRow].id);
    }
    //print_element_ids();
}

void cNewShopWindow::on_btn_del_clicked() {
    qDebug() << "on_btn_del_clicked";
    QList<QTableWidgetItem*> selectItem = ui->tableWidget_right->selectedItems();
    for(int i=0; i<selectItem.size(); i++) {
        qDebug() << selectItem[i]->text();
        int selRow = ui->tableWidget_right->row(selectItem[i]);
        ui->tableWidget_right->removeRow(selRow);
        model_ids.removeAt(selRow);
    }
    //print_element_ids();
}

void cNewShopWindow::on_buttonBox_accepted() {
    shpName = ui->textEdit_shopName->toPlainText();
    if(shpName == "" || model_ids.size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("Error !");
        msgBox.exec();
    }
    else
        emit newShopAccepted(plantId, shpName, model_ids);
}

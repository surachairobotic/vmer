#include "cNewModelWindow.h"
#include "ui_cNewModelWindow.h"
#include <QMessageBox>

cNewModelWindow::cNewModelWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cNewModelWindow)
{
    ui->setupUi(this);
    mdlName = "";
}

cNewModelWindow::~cNewModelWindow() {
    delete ui;
}

void cNewModelWindow::createElementTable() {
    int row = db->elements.size();

    ui->tableWidget_left->setColumnCount(1);
    QStringList labels;
    labels << tr("Element Name");
    ui->tableWidget_left->setHorizontalHeaderLabels(labels);
    ui->tableWidget_left->horizontalHeader()->setStretchLastSection(true);

    QHeaderView *vh=new QHeaderView(Qt::Vertical);
    vh->hide();
    ui->tableWidget_left->setVerticalHeader(vh);

    ui->tableWidget_left->setRowCount(row);
    ui->tableWidget_left->setColumnCount(1);
    for(int i=0; i<row; i++) {
        QString txt = db->elements[i].name;
        ui->tableWidget_left->setItem(i, 0, new QTableWidgetItem(txt));
    }
    ui->tableWidget_right->setColumnCount(1);
    ui->tableWidget_right->setHorizontalHeaderLabels(labels);
    ui->tableWidget_right->horizontalHeader()->setStretchLastSection(true);
    vh=new QHeaderView(Qt::Vertical);
    vh->hide();
    ui->tableWidget_right->setVerticalHeader(vh);
}

void cNewModelWindow::on_btn_add_clicked() {
    qDebug() << "on_btn_add_clicked";
    QList<QTableWidgetItem*> selectItem = ui->tableWidget_left->selectedItems();
    for(int i=0; i<selectItem.size(); i++) {
        qDebug() << selectItem[i]->text();
        int row = ui->tableWidget_right->rowCount();
        ui->tableWidget_right->setRowCount(row+1);
        ui->tableWidget_right->setItem(row, 0, new QTableWidgetItem(selectItem[i]->text()));
        int selRow = ui->tableWidget_left->row(selectItem[i]);
        //qDebug() << "selRow : " << selRow;
        element_ids.push_back(db->elements[selRow].id);
    }
    print_element_ids();
}

void cNewModelWindow::on_btn_del_clicked() {
    qDebug() << "on_btn_del_clicked";
    QList<QTableWidgetItem*> selectItem = ui->tableWidget_right->selectedItems();
    for(int i=0; i<selectItem.size(); i++) {
        qDebug() << selectItem[i]->text();
        int selRow = ui->tableWidget_right->row(selectItem[i]);
        ui->tableWidget_right->removeRow(selRow);
        element_ids.removeAt(selRow);
    }
    print_element_ids();
}

void cNewModelWindow::print_element_ids() {
    qDebug() << "print_element_ids";
    for(int i=0; i<element_ids.size(); i++)
        qDebug() << element_ids[i];
}

void cNewModelWindow::on_buttonBox_accepted() {
    mdlName = ui->textEdit_modelName->toPlainText();
    if(mdlName == "" || element_ids.size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("Error !");
        msgBox.exec();
    }
    else
        emit newModelAccepted(mdlName, element_ids);
}

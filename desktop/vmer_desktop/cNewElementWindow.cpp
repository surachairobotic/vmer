#include "cNewElementWindow.h"
#include "ui_cNewElementWindow.h"
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QDir>
#include <QFileDialog>
#include <QtDebug>
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>

cNewElementWindow::cNewElementWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cNewElementWindow)
{
    ui->setupUi(this);
    ui->textEdit->setText("Element_01");
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

cNewElementWindow::~cNewElementWindow()
{
    delete ui;
}

void cNewElementWindow::on_btn_addPoint_clicked()
{
    //QTableWidgetItem *itm = new QTableWidgetItem();
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row+1);
}

void cNewElementWindow::on_btn_addTemplate_clicked() {

}

void cNewElementWindow::add_graphics_standard()
{
    /*
    QString hostname = QDir::home().dirName();
    QString path = "C:/Users/"+hostname+"/Documents/VmerProjects/";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file."), path,
                                                    tr("Image Files (*.png *.jpg *.jpeg)"));
    qDebug() << fileName;
    QGraphicsScene *scene = new QGraphicsScene(this);
    QPixmap *pm = new QPixmap(fileName);
    *pm = pm->scaled(ui->graphics_standard->width(), ui->graphics_standard->height(), Qt::KeepAspectRatio);
    scene->addPixmap(*pm);
    ui->graphics_standard->setScene(scene);
    ui->graphics_standard->show();
    */
}

void cNewElementWindow::on_buttonBox_accepted()
{
    if(ui->textEdit->toPlainText() == "") {
        QMessageBox msgBox;
        msgBox.setText("");
        msgBox.exec();
    }
    else
        emit newElementAccepted(5, 6, 7);
}

void cNewElementWindow::on_btn_editImage_clicked()
{
    QString hostname = QDir::home().dirName();
    QString path = "C:/Users/"+hostname+"/Documents/VmerProjects/";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file."), path,
                                                    tr("Image Files (*.png *.jpg *.jpeg)"));
    qDebug() << fileName;
    QGraphicsScene *scene = new QGraphicsScene(this);
    QPixmap *pm = new QPixmap(fileName);
    *pm = pm->scaled(ui->graphics_user->width(), ui->graphics_user->height(), Qt::KeepAspectRatio);
    scene->addPixmap(*pm);
    ui->graphics_user->setScene(scene);
    ui->graphics_user->show();
}

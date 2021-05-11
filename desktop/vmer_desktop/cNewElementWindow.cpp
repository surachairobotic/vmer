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

    eleName = stdImg = userImg = "";

    //connect(ui->btn_stdImage, SIGNAL(clicked()), SLOT(on_btn_stdImage_clicked()));
    //connect(ui->btn_userImage, SIGNAL(clicked()), SLOT(on_btn_userImage_clicked()));
    //connect(ui->btn_addPoint, SIGNAL(clicked()), SLOT(on_btn_addPoint_clicked()));
}

cNewElementWindow::~cNewElementWindow() {
    delete ui;
}

void cNewElementWindow::on_btn_addPoint_clicked() {
    //QTableWidgetItem *itm = new QTableWidgetItem();
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row+1);
}

void cNewElementWindow::on_btn_stdImage_clicked() {
    stdImg = img_browse(currProjImage+"standard_elements/", ui->graphics_standard);
}

void cNewElementWindow::add_graphics(QString fname, QGraphicsView *gView) {
    QGraphicsScene *scene = new QGraphicsScene(this);
    QPixmap *pm = new QPixmap(fname);
    *pm = pm->scaled(gView->width(), gView->height(), Qt::KeepAspectRatio);
    scene->addPixmap(*pm);
    gView->setScene(scene);
    gView->show();
}

void cNewElementWindow::on_buttonBox_accepted() {
    eleName = ui->textEdit->toPlainText();
    if(eleName == "" || (stdImg == "" && userImg == "")) {
        QMessageBox msgBox;
        msgBox.setText("Error !");
        msgBox.exec();
    }
    else {
        int row = ui->tableWidget->rowCount();
        for(int i=0; i<row; i++) {
            QTableWidgetItem *itm = ui->tableWidget->takeItem(i, 0);
            QString name="";
            if(itm) {
                name = itm->text();
                if(name != "")
                    pntName.push_back(name);
            }
        }
        emit newElementAccepted(eleName, stdImg, userImg, pntName);
    }
}

void cNewElementWindow::on_btn_userImage_clicked() {
    userImg = img_browse(currProjImage+"user_elements/", ui->graphics_user);
}

QString cNewElementWindow::img_browse(QString path, QGraphicsView *gView) {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file."), path,
                                                    tr("Image Files (*.png *.jpg *.jpeg)"));
    add_graphics(fileName, gView);
    return fileName;
}

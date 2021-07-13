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
#include <QFile>

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

void cNewElementWindow::initPointTable() {
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    for(int i=0; i<pntName.size(); i++) {
        ui->tableWidget->insertRow(i);
        QTableWidgetItem *itm = new QTableWidgetItem(pntName[i]);
        ui->tableWidget->setItem(i, 0, itm);
    }
}

void cNewElementWindow::on_btn_stdImage_clicked() {
    //stdImg = img_browse(currProjImage+"standard_elements/", ui->graphics_standard);
    QString path = currProjImage+"standard_elements/";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file."), path,
                                                    tr("Image Files (*.png *.jpg *.jpeg)"));
    qDebug() << fileName;
    QString imgFName = fileName;
    QString pntFName = fileName.split('.')[0]+".point";
    QString descFName = fileName.split('.')[0]+".txt";
    qDebug() << pntFName;
    qDebug() << descFName;
    if( QFile::exists(imgFName) && QFile::exists(pntFName) && QFile::exists(descFName) ) {
        qDebug() << "QFile::exists(imgFName) : " << QFile::exists(imgFName);
        qDebug() << "QFile::exists(pntFName) : " << QFile::exists(pntFName);
        qDebug() << file2String(pntFName);
        qDebug() << "QFile::exists(descFName) : " << QFile::exists(descFName);
        qDebug() << file2String(descFName);
        add_graphics(imgFName, ui->graphics_standard);
        stdImg = imgFName;
        descElement = file2String(descFName);
        QList<QString> tmp = file2String(pntFName).split('\n', Qt::SkipEmptyParts);
        pntName.clear();
        pntDesc.clear();
        for(int i=0; i<tmp.size(); i++) {
            int c_indx = tmp[i].indexOf(',');
            bool vha[3] = {1,1,1};
            if(c_indx == -1) {
                pntName.push_back(tmp[i]);
            }
            else {
                pntName.push_back(tmp[i].split(',', Qt::SkipEmptyParts)[0]);
                vha[0] = tmp[i].contains('V', Qt::CaseInsensitive);
                vha[1] = tmp[i].contains('H', Qt::CaseInsensitive);
                vha[2] = tmp[i].contains('A', Qt::CaseInsensitive);
            }
            QString msg = QString("{\"V\":%1,\"H\":%2,\"A\":%3}").arg(QString::number(vha[0]),QString::number(vha[1]),QString::number(vha[2]));
            pntDesc.push_back(msg);
            qDebug() << QString("[%1] : %2 <--> %3").arg(QString::number(i),tmp[i],msg);
        }
        initPointTable();
    }
    else {
        qDebug() << "Standard file is incorrect.";
        QMessageBox msgBox;
        msgBox.setText("Standard file is incorrect.");
        msgBox.exec();
    }
    /*
    QString eleName, stdImg, userImg, descElement;
    QList<QString> pntName, pntDesc;
    */
}

QString cNewElementWindow::file2String(QString fname) {
    QFile f(fname);
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return QString("Can't open this file : " + fname);
    QTextStream in(&f);
    return in.readAll();
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
        pntName.clear();
        for(int i=0; i<row; i++) {
            QTableWidgetItem *itm = ui->tableWidget->takeItem(i, 0);
            QString name="";
            if(itm) {
                name = itm->text();
                if(name != "")
                    pntName.push_back(name);
            }
        }
        for(int i=pntDesc.size(); i<row; i++) {
            pntDesc.push_back("{\"V\":1,\"H\":1,\"A\":1}");
        }
        emit newElementAccepted(eleName, stdImg, userImg, descElement, pntName, pntDesc);
    }
}

void cNewElementWindow::on_btn_userImage_clicked() {
    userImg = img_browse(currProjImage+"user_elements/", ui->graphics_user);
}

QString cNewElementWindow::img_browse(QString path, QGraphicsView *gView) {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file."), path,
                                                    tr("Image Files (*.png *.jpg *.jpeg)"));
    qDebug() << fileName;
    add_graphics(fileName, gView);
    return fileName;
}

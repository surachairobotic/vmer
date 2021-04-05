#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"

void MainWindow::on_actionNew_Element_Window_triggered()
{
    newEleWin = new cNewElementWindow(this);
    //QObject::connect(newprojDialog, SIGNAL(accepted(QString, QString)), this, SLOT(newprojDialog_accepted(QString, QString)));
    //QObject::connect(newprojDialog, SIGNAL(rejected()), this, SLOT(newprojDialog_rejected()));



    newEleWin->show();
    qDebug() << "END : on_actionNew_Element_Window_triggered";
}

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"

void MainWindow::on_actionNew_Model_Window_triggered()
{
    newMdlWin = new cNewModelWindow(this);
    newMdlWin->db = this->db;
    newMdlWin->createElementTable();
    //QObject::connect(newprojDialog, SIGNAL(accepted(QString, QString)), this, SLOT(newprojDialog_accepted(QString, QString)));
    //QObject::connect(newprojDialog, SIGNAL(rejected()), this, SLOT(newprojDialog_rejected()));
    newMdlWin->show();
}

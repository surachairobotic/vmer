#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QFileDialog"
#include "QDebug"
#include "cDB.h"

void MainWindow::on_actionNew_Project_triggered()
{
    newprojDialog = new cNewProjectDialog(this);
    QObject::connect(newprojDialog, SIGNAL(accepted(QString, QString)), this, SLOT(newprojDialog_accepted(QString, QString)));
    QObject::connect(newprojDialog, SIGNAL(rejected()), this, SLOT(newprojDialog_rejected()));
    newprojDialog->show();
}

void MainWindow::newprojDialog_accepted(const QString fname, const QString fpath) {
    qDebug() << "MainWindow::on_cNewProjectDialog_accepted";
    qDebug() << fname;
    qDebug() << fpath;
    //currProjName = new QString(fname);
    //currProjPath = new QString(fpath);
    currProjName = fname;
    currProjPath = fpath;
    currProjImage = (fpath+"images/");
    QDir *pathDir = new QDir(currProjPath);
    if(!pathDir->exists())
        pathDir->mkpath(currProjPath);
    pathDir = new QDir(currProjImage);
    if(!pathDir->exists())
        pathDir->mkpath(currProjImage);

    cNewProjectDialogTerminate();
    openDB(&currProjName, &currProjPath, true);
}
void MainWindow::newprojDialog_rejected() {
    qDebug() << "MainWindow::cNewProjectDialog_rejected";
    cNewProjectDialogTerminate();
}
void MainWindow::cNewProjectDialogTerminate() {
    qDebug() << "MainWindow::cNewProjectDialogTerminate()";
    if(newprojDialog != NULL) {
        newprojDialog->close();
    }
    delete newprojDialog;
}




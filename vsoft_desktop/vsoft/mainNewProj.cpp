#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QDebug"
#include "cDB.h"
#include "commonFunction.h"

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
    realProjName = fname;
    realProjPath = fpath;
    realProjImage = (fpath+"images/");
//    QDir *pathDir = new QDir(realProjPath);
//    if(!pathDir->exists())
//        pathDir->mkpath(realProjPath);
//    pathDir = new QDir(realProjImage);
//    if(!pathDir->exists())
//        pathDir->mkpath(realProjImage);

    commonFunction::cpDir("C:/vmer/vsoft_desktop/vsoft/images", realProjImage);

    QList<QString> paths = {realProjPath,
                            realProjImage,
                            realProjImage + "user_elements/",
                            realProjImage + "standard_elements/"};
    for(int i=0; i<paths.size(); i++) {
        commonFunction::mkpath(paths[i]);
    }

    //qDebug() << realProjName;
    //qDebug() << realProjPath.section('/', 0, -3)+'/';
    //qDebug() << realProjImage;
    openDB(&realProjName, &realProjPath, true);
    openProject(realProjName,
                realProjPath,
                true);
    //openDB(&currProjName, &currProjPath, true);

    cNewProjectDialogTerminate();
    //openDB(&currProjName, &currProjPath, true);
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




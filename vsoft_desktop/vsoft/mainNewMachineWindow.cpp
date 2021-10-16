#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "cNewMachineWindow.h"
#include "commonFunction.h"

void MainWindow::on_actionNew_Machine_Window_triggered(int shpId)
{
    newMachineWin = new cNewMachineWindow(this);
    newMachineWin->setWindowTitle("New Machine");
    newMachineWin->db = this->db;
    newMachineWin->initData();
    newMachineWin->shpId = shpId;
//    newMachineWin->currProjName = currProjName;
//    newMachineWin->currProjPath = currProjPath;
//    newMachineWin->currProjImage = currProjImage;

    //newEleWin->setMainwindow(this);
    //QObject::connect(newprojDialog, SIGNAL(accepted(QString, QString)), this, SLOT(newprojDialog_accepted(QString, QString)));
    //QObject::connect(newprojDialog, SIGNAL(rejected()), this, SLOT(newprojDialog_rejected()));

    connect(newMachineWin, &cNewMachineWindow::newMachineAccepted, this, &MainWindow::newMachineMain);

    newMachineWin->show();
    qDebug() << "END : on_actionNew_Machine_Window_triggered";
}

void MainWindow::newMachineMain(QString name, int mdlId, int shpId, QString serial, QString desc) {
    int id = commonFunction::findMaxId(&db->machines)+1;
    cMachine *machine = new cMachine(id, mdlId, shpId, name, serial, desc);
    db->insert(machine);
    displayDBTree();
    displayRouteTree();
}

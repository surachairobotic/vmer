#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"

void MainWindow::on_actionNew_Model_Window_triggered()
{
    newMdlWin = new cNewModelWindow(this);
    newMdlWin->setWindowTitle("New Model");
    newMdlWin->db = this->db;
    newMdlWin->createElementTable();
    //QObject::connect(newprojDialog, SIGNAL(accepted(QString, QString)), this, SLOT(newprojDialog_accepted(QString, QString)));
    //QObject::connect(newprojDialog, SIGNAL(rejected()), this, SLOT(newprojDialog_rejected()));

    connect(newMdlWin, &cNewModelWindow::newModelAccepted, this, &MainWindow::newModelMain);

    newMdlWin->show();
}

void MainWindow::newModelMain(QString name, QList<int> eleIds) {
    qDebug() << "newModelMain";
    newModel(name, eleIds);
}

bool MainWindow::newModel() {
    qDebug("newModel");
    int idM=1;
    for(int i=0; i<db->models.size(); i++) {
        if(idM <= db->models[i].id)
            idM = db->models[i].id;
    }
    idM+=1;

    // prob of number 1 is 3/5
    int numElement = (QRandomGenerator::global()->generate() % 5) + 1;
    if(numElement > 3)
        numElement = 1;
    QList<int> element_id;
    for(int i=0; i<numElement; i++) {
        int indx = QRandomGenerator::global()->generate() % db->elements.size();
        element_id.push_back(db->elements[indx].id);
    }

    qDebug() << idM << ", " << "model_"+QString::number(idM) << ", " << element_id.size();
    db->insert_model(idM, "model_"+QString::number(idM), "^_model_^", element_id);
    return false;
}

bool MainWindow::newModel(QString name, QList<int> eleIds) {
    int idM=1;
    for(int i=0; i<db->models.size(); i++) {
        if(idM <= db->models[i].id)
            idM = db->models[i].id;
    }
    idM+=1;
    qDebug() << idM << ", " << "model_"+QString::number(idM) << ", " << eleIds.size();
    db->insert_model(idM, name, "^_model_^", eleIds);
    displayModelTree();
    return true;
}

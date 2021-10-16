#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

void MainWindow::on_actionNew_Element_Window_triggered()
{
    newEleWin = new cNewElementWindow(this);
    newEleWin->setWindowTitle("New Element");
    newEleWin->currProjName = currProjName;
    newEleWin->currProjPath = currProjPath;
    newEleWin->currProjImage = currProjImage;

    //newEleWin->setMainwindow(this);
    //QObject::connect(newprojDialog, SIGNAL(accepted(QString, QString)), this, SLOT(newprojDialog_accepted(QString, QString)));
    //QObject::connect(newprojDialog, SIGNAL(rejected()), this, SLOT(newprojDialog_rejected()));

    connect(newEleWin, &cNewElementWindow::newElementAccepted, this, &MainWindow::newElementMain);

    newEleWin->show();
    qDebug() << "END : on_actionNew_Element_Window_triggered";
}

void MainWindow::newElementMain(QString name, QString stdImg, QString userImg, QString descElement, QList<QString> pntName, QList<QString> pntDesc) {
    if(userImg != "") {
        QString usrImgName = currProjImage+userImg.section('/',-2,-1);
        if( QFile::exists(usrImgName) ) {
            QString newName;
            int c=1;
            do {
                newName = usrImgName.section('.',0,-2) + '(' + QString::number(c) + ")." + usrImgName.section('.',-1);
                c++;
                qDebug() << "In Loop : " << newName;
            } while( QFile::exists(newName) );
            qDebug() << "Selected : " << newName;
            QFile::copy(userImg, newName);
        }
    }
    newElement(name, stdImg.section('/',-1), userImg.section('/',-1), descElement, pntName, pntDesc);
}

bool MainWindow::newElement(QString name, QString stdImg, QString userImg, QString descElement, QList<QString> pntName, QList<QString> pntDesc) {
    qDebug("newElement");
    int idE=1;
    for(int i=0; i<db->elements.size(); i++) {
        if(idE<=db->elements[i].id)
            idE=db->elements[i].id;
    }
    idE+=1;
    qDebug() << currProjImage;
    //QDir *pathDir = new QDir(currProjImage);
    //QStringList img_names = pathDir->entryList(QDir::Files);
    //int value = QRandomGenerator::global()->generate() % img_names.size();
    //cElement *ele = new cElement(idE, "element_"+QString::number(idE), img_names[value], "");

    cElement *ele = new cElement(idE, name, stdImg, userImg, descElement);
    db->insert(ele);
    //int value = QRandomGenerator::global()->generate() % 7 + 1;
    //value = 0; // for Debug
    //qDebug() << "Value : " << value;
    int idP = 1;
    for(int i=0; i<db->points.size(); i++) {
        if(idP<=db->points[i].id)
            idP=db->points[i].id;
    }
    //for(int num=1; num<value; num++) {
    for(int i=0; i<pntName.size(); i++) {
        idP+=1;
        //cPoint *pnt = new cPoint(idP, idE, "point_"+QString::number(num), "{\"V\":1,\"H\":0}", "");
        cPoint *pnt = new cPoint(idP, idE, pntName[i], pntDesc[i], "");
        db->insert(pnt);
    }
    displayElementTree();
    return false;
}

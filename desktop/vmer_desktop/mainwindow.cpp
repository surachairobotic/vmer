#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QFileDialog"
#include "QDebug"
#include "cDB.h"
#include "QSize"
#include "QTreeWidget"
#include "QTreeWidgetItem"
#include "QPushButton"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QIcon"
#include <QRandomGenerator>
#include <QPixmap>
#include <QGraphicsView>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , hasDB(false)
{
    ui->setupUi(this);
    ui->actionClose_Project->setEnabled(false);
    QSize size = this->size();
    int width = size.width();
    ui->splitter->setSizes(QList<int>({width/3, width/3*2}));

    ui->treeWidgetDB->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidgetDB, &QTreeWidget::customContextMenuRequested, this, &MainWindow::dbRightClickMenu);

    ui->treeWidgetModel->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidgetModel, &QTreeWidget::customContextMenuRequested, this, &MainWindow::modelRightClickMenu);

    ui->treeWidgetElement->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidgetElement, &QTreeWidget::customContextMenuRequested, this, &MainWindow::elementRightClickMenu);
    this->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::openDB(const QString *fname, const QString *fpath, const bool script) {
    QString *dbFile = new QString(*fpath+*fname+".sqlite3");
    return openDB(dbFile, script);
}
bool MainWindow::openDB(const QString *fname, const bool script) {
    db = new cDB(*fname);
    hasDB = true;
    ui->actionClose_Project->setEnabled(true);
    if(script) {
        db->load_script_file(":/db_scripts/vmer_sqlite_script.sql");
    }
    return true;
}

void MainWindow::on_actionClose_Project_triggered() {
    if(hasDB) {
        delete db;
        hasDB = false;
        //delete currProjName;
        //delete currProjPath;
        currProjName = currProjPath = currProjImage = "";
        ui->treeWidgetDB->clear();
        ui->treeWidgetElement->clear();
        ui->treeWidgetModel->clear();
        ui->actionClose_Project->setEnabled(false);
    }
}

void MainWindow::on_actionOpen_triggered() {
    QString hostname = QDir::home().dirName();
    QString path = "C:/Users/"+hostname+"/Documents/VmerProjects/";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), path,
                                                    tr("Project Files (*.sqlite3)"));
    qDebug() << fileName;
    realProjName = fileName.section('/', -2, -2);
    realProjPath = fileName.section('/', 0, -2) + "/";
    realProjImage = (realProjPath+"images/");

    currProjName = realProjName;
    currProjPath = path + "vmer_tmp/" + currProjName + '/';
    currProjImage = (currProjPath+"images/");

    qDebug() << "currProjName : " << currProjName;
    qDebug() << "currProjPath : " << currProjPath;
    qDebug() << "currProjImage : " << currProjImage;

    cpDir(realProjPath, currProjPath);

    fileName = currProjPath + '/' + currProjName + ".sqlite3";
    openDB(&fileName, false);
    updateDatabase();

    displayDBTree();
    displayElementTree();
    displayModelTree();
}

bool MainWindow::rmDir(const QString &dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists())
        return true;
    foreach(const QFileInfo &info, dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        if (info.isDir()) {
            if (!rmDir(info.filePath()))
                return false;
        } else {
            if (!dir.remove(info.fileName()))
                return false;
        }
    }
    QDir parentDir(QFileInfo(dirPath).path());
    return parentDir.rmdir(QFileInfo(dirPath).fileName());
}

bool MainWindow::cpDir(const QString &srcPath, const QString &dstPath)
{
    rmDir(dstPath);
    QDir parentDstDir(QFileInfo(dstPath).path());
    //QString strDstPath = QFileInfo(dstPath).fileName();
    if (!parentDstDir.mkpath(dstPath))
        return false;

    QDir srcDir(srcPath);
    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        int chk=0;
        if (info.isDir()) {
            if (!cpDir(srcItemPath, dstItemPath)) {
                return false;
            }
            chk=1;
        } else if (info.isFile()) {
            if (!QFile::copy(srcItemPath, dstItemPath)) {
                return false;
            }
            chk=2;
        } else {
            qDebug() << "Unhandled item" << info.filePath() << "in cpDir";
            chk=3;
        }
        qDebug() << "chk = " << chk;
    }
    return true;
}
void MainWindow::on_treeWidgetElement_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(hasDB) {
        if(current == nullptr)
            return ;
        QString msg = current->whatsThis(0);
        QString prev = "NULL";
        if(previous != nullptr)
            prev = previous->whatsThis(0);
        qDebug() << prev << ", " << msg;
        if(current->whatsThis(0).contains("Element")) {
            cElementWidget *elm = static_cast<cElementWidget*>(current);
            //elm->cParent->printInfo();
            elementGraphics(elm->cParent);
        }
        else if(current->whatsThis(0).contains("Point")) {
            cPointWidget *pnt = static_cast<cPointWidget*>(current);
            //pnt->cParent->printInfo();
        }
    }
}
void MainWindow::elementGraphics(const cElement *ele, bool del) {
    QString imgName = ele->image;
    img = new QPixmap(currProjImage+imgName);
    hLayout = new QHBoxLayout();
    gView = new QGraphicsView();
    scene = new QGraphicsScene();

    scene->addPixmap(*img);

    QRectF rect = scene->itemsBoundingRect();
    scene->setSceneRect(rect);

    gView->setScene(scene);
    gView->fitInView(rect, Qt::KeepAspectRatio);
    gView->show();
    hLayout->addWidget(gView);

    int row=ele->points.size();
    QTableWidget *pntTable = new QTableWidget(row+1, 1);
    pntTable->setHorizontalHeaderLabels({"Point ID"});
    for(int i=0; i<row; i++) {
        QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(ele->points[i]->name));
        pntTable->setItem(i, 1, newItem);
    }
    hLayout->addWidget(pntTable);


    if(del) {
        QLayoutItem* item;
        while ( del && ( item = ui->vLayoutElementInfo->layout()->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }

    ui->vLayoutElementInfo->addLayout(hLayout);
}

void MainWindow::elementRightClickMenu(const QPoint &pos)
{
    hasDB = false;
    QTreeWidget *tree = ui->treeWidgetElement;
    QTreeWidgetItem *item = tree->itemAt(pos);

    qDebug()<<pos<<item->text(0);
    bool bUpdate=false;

    if(item->whatsThis(0).contains("Element")) {
        newAct = new QAction("New Point", this);
        newAct->setStatusTip("New Point xxxxxxxxxxxxxxx");
        delAct = new QAction("Delete Element", this);
        delAct->setStatusTip("Delete Element xxxxxxxxxxxxxxx");

        QMenu menu(this);
        menu.addAction(newAct);
        menu.addAction(delAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected && selected->text() == "New Point") {
            qDebug() << "selected : " << selected->text();
            cElementWidget* elmWdg = static_cast<cElementWidget*>(item);
            int elmId = elmWdg->cParent->id;
            newPoint(elmId);
            bUpdate=true;

            /*
            updateDatabase();
            QTreeWidgetItem *parent = item->parent();
            int itemIndex = parent->indexOfChild(item);
            parent->removeChild(item);
            parent->insertChild(itemIndex, elmWdg->cParent->get_widget());
            displayElementTree();
            */
        }
        else if(selected && selected->text() == "Delete Element") {
            qDebug() << "selected : " << selected->text();
            cElementWidget* elmWdg = static_cast<cElementWidget*>(item);
            int elmId = elmWdg->cParent->id;
            /*
            delElement(elmWdg);
            for(int i=0; i<elmWdg->cParent->points.size(); i++) {
                int _id = elmWdg->cParent->points[i]->id;
                db->deleteDB("point", _id);
            }
            db->deleteDB("element", elmId);
            */
            db->delete_element(elmId);
            bUpdate=true;

            /*
            updateDatabase();
            QTreeWidgetItem *parent = item->parent();
            int itemIndex = parent->indexOfChild(item);
            parent->removeChild(item);
            parent->insertChild(itemIndex, elm->cParent->get_widget());
            displayElementTree();
            */
        }
    }
    else if(item->whatsThis(0).contains("Point")) {
        delAct = new QAction("Delete Point", this);
        delAct->setStatusTip("Delete Point xxxxxxxxxxxxxxx");
        QMenu menu(this);
        menu.addAction(delAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected->text() == "Delete Point") {
            qDebug() << "selected : " << selected->text();
            cPointWidget* pntWdg = static_cast<cPointWidget*>(item);
            int pntId = pntWdg->cParent->id;
            //db->deleteDB("point", pntId);
            //delPoint(pntWdg);
            db->delete_point(pntId);
            bUpdate=true;
        }
    }
    else {
        newAct = new QAction("New Element", this);
        newAct->setStatusTip("New Element xxxxxxxxxxxxxxx");

        QMenu menu(this);
        menu.addAction(newAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected->text() == "New Element") {
            qDebug() << "selected" << selected->text();
            //newElement();
            //bUpdate=true;
            on_actionNew_Element_Window_triggered();
        }
    }
    if(bUpdate)
        displayElementTree();
    hasDB = true;
}
bool MainWindow::newElement() {
    int idE=1;
    for(int i=0; i<db->elements.size(); i++) {
        if(idE<=db->elements[i].id)
            idE=db->elements[i].id;
    }
    idE+=1;
    qDebug() << currProjImage;
    QDir *pathDir = new QDir(currProjImage);
    QStringList img_names = pathDir->entryList(QDir::Files);
    int value = QRandomGenerator::global()->generate() % img_names.size();
    cElement *ele = new cElement(idE, "element_"+QString::number(idE), img_names[value], "");
    db->insert(ele);
    value = QRandomGenerator::global()->generate() % 7 + 1;
    qDebug() << "Value : " << value;
    int idP = 1;
    for(int i=0; i<db->points.size(); i++) {
        if(idP<=db->points[i].id)
            idP=db->points[i].id;
    }
    for(int num=1; num<value; num++) {
        idP+=1;
        cPoint *pnt = new cPoint(idP, idE, "point_"+QString::number(num), "{\"V\":1,\"H\":0}", "");
        db->insert(pnt);
    }
    displayElementTree();
    return false;
}
void MainWindow::newElementMain(int x, int y, int z) {
    qDebug() << "X: " << x << ", Y: " << y << ", Z: " << z;
}

bool MainWindow::newPoint(int element_id) {
    int idP = 1;
    for(int i=0; i<db->points.size(); i++) {
        if(idP<=db->points[i].id)
            idP=db->points[i].id;
    }
    idP+=1;
    cPoint *pnt = new cPoint(idP, element_id, "point_"+QString::number(idP), "{\"V\":1,\"H\":0}", "");
    db->insert(pnt);
    //displayElementTree();
    return false;
}
void MainWindow::delPoint(QTreeWidgetItem *item) {
    qDebug() << "delPoint : " << item->text(0);

}
void MainWindow::delElement(QTreeWidgetItem *item) {
    qDebug() << "delElement : " << item->text(0);
}

void MainWindow::modelRightClickMenu(const QPoint &pos)
{
    qDebug() << "modelRightClickMenu";
    hasDB = false;
    QTreeWidget *tree = ui->treeWidgetModel;
    QTreeWidgetItem *item = tree->itemAt(pos);

    qDebug()<<pos<<item->text(0);
    bool bUpdate=false;

    if(item->whatsThis(0).contains("Element") || item->whatsThis(0).contains("Point")) {
        qDebug() << "contains(...) : " << item->whatsThis(0);
    }
    else if(item->whatsThis(0).contains("Model")) {
        qDebug() << "contains(Model) : " << item->whatsThis(0);
        delAct = new QAction("Delete Model", this);
        delAct->setStatusTip("Delete Model xxxxxxxxxxxxxxx");
        QMenu menu(this);
        menu.addAction(delAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected->text() == "Delete Model") {
            qDebug() << "selected : " << selected->text();
            cModelWidget* pntWdg = static_cast<cModelWidget*>(item);
            //int pntId = pntWdg->cParent->id;
            //db->deleteDB("model", pntId);
            //delPoint(pntWdg);
            db->delete_model(pntWdg->cParent->id);
            bUpdate=true;
        }
    }
    else {
        qDebug() << "else : " << item->whatsThis(0);
        newAct = new QAction("New Model", this);
        newAct->setStatusTip("New Model xxxxxxxxxxxxxxx");

        QMenu menu(this);
        menu.addAction(newAct);

        QPoint pt(pos);
        QAction *selected = menu.exec( tree->mapToGlobal(pt) );
        if (selected->text() == "New Model") {
            qDebug() << "selected" << selected->text();
            on_actionNew_Model_Window_triggered();
            //newModel();
            //bUpdate=true;
        }
    }
    if(bUpdate)
        displayModelTree();
    hasDB = true;
}
bool MainWindow::newModel() {
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

void MainWindow::dbRightClickMenu(const QPoint &pos)
{
    qDebug() << "dbRightClickMenu";
    hasDB = false;
    QTreeWidget *tree = ui->treeWidgetDB;
    QTreeWidgetItem *item = tree->itemAt(pos);

    qDebug()<<pos<<item->text(0);
    bool bUpdate=false;

    if(item->whatsThis(0).contains("DBTable")) {
        qDebug() << "contains(DBTable) : " << item->whatsThis(0);
    }
    else if(item->whatsThis(0).contains("Company")) {
        qDebug() << "contains(Company) : " << item->whatsThis(0);
    }
    else if(item->whatsThis(0).contains("Plant")) {
        qDebug() << "contains(Plant) : " << item->whatsThis(0);
    }
    else if(item->whatsThis(0).contains("Shop")) {
        qDebug() << "contains(Shop) : " << item->whatsThis(0);
    }
    else {
        qDebug() << "else : " << item->whatsThis(0);
    }
    if(bUpdate)
        displayDBTree();
    hasDB = true;
}

void MainWindow::on_treeWidgetModel_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    qDebug() << "on_treeWidgetModel_currentItemChanged";
    if(hasDB) {
        for(int i=0; i<db->models.size(); i++) {
            qDebug() << "model_id: " << db->models[i].id << ", " << &db->models[i];
        }
        for(int i=0; i<db->elements.size(); i++) {
            qDebug() << "element_id: " << db->elements[i].id << ", " << &db->elements[i];
        }

        if(current == nullptr)
            return ;
        QString msg = current->whatsThis(0);
        QString prev = "NULL";
        if(previous != nullptr)
            prev = previous->whatsThis(0);
        qDebug() << prev << ", " << msg;
        if(current->whatsThis(0).contains("Element")) {
            cElementWidget *elm = static_cast<cElementWidget*>(current);
            //elm->cParent->printInfo();
            elementGraphics(elm->cParent);
        }
        else if(current->whatsThis(0).contains("Model")) {
            cModelWidget *mdl = static_cast<cModelWidget*>(current);
            qDebug() << "address of mdl->cParent : " << mdl->cParent;
            mdl->cParent->printInfo("on_treeWidgetModel_currentItemChanged");
            bool bStart=true;
            for(int i=0; i<mdl->cParent->elements.size(); i++) {
                elementGraphics(mdl->cParent->elements[i], bStart);
                bStart=false;
            }
        }
    }
}

void MainWindow::on_actionSave_Project_triggered()
{
    cpDir(currProjPath, realProjPath);
}

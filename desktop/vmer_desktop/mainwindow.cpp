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
    ui->treeWidgetElement->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidgetModel, &QTreeWidget::customContextMenuRequested, this, &MainWindow::modelRightClickMenu);
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
    currProjName = fileName.section('/', -2, -2);
    currProjPath = fileName.section('/', 0, -2) + "/";
    currProjImage = (currProjPath+"images/");

    qDebug() << "currProjName : " << currProjName;
    qDebug() << "currProjPath : " << currProjPath;
    qDebug() << "currProjImage : " << currProjImage;
    openDB(&fileName, false);
    updateDatabase();
    //displayDBTree();
    displayModelTree();
    displayElementTree();
}

bool MainWindow::displayDBTree()
{
    QList<cDBTable> *db_table = new QList<cDBTable>();
    qDebug() << "1. db_table->size() : " << db_table->size();
    if(!db->get_db_table(db_table))
        return false;
    qDebug() << "2. db_table->size() : " << db_table->size();

    if(db_table->size() != 1)
        return false;

    //QTreeWidgetItem *root = static_cast<QTreeWidgetItem*>(&(*db_table)[0]);
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetDB);
    root->setText(0, (*db_table)[0].name);
    ui->treeWidgetDB->addTopLevelItem(root);
    //ui->treeWidgetDB->addTopLevelItem(root);
    //root->addChild(static_cast<QTreeWidgetItem*>(&(*db_table)[0]));

    for(int i=0; i<db->companies.size(); i++) {
        root->addChild(static_cast<QTreeWidgetItem*>(&db->companies[i]));
    }

    /*
    for(int i=0; i<db_table->size(); i++) {
        QList<cCompany> *company = new QList<cCompany>();
        if(!db->get_company(company, (*db_table)[i].id))
            continue;
        for(int j=0; j<company->size(); j++) {
            QTreeWidgetItem *childCompany = new QTreeWidgetItem();
            childCompany->setText(0, (*company)[j].name);
            root->addChild(static_cast<QTreeWidgetItem*>(&childCompany);
            QList<cPlant> *plant = new QList<cPlant>();
            if(!db->get_plant(plant, (*company)[j].id))
                continue;
            for(int k=0; k<plant->size(); k++) {
                QTreeWidgetItem *childPlant = new QTreeWidgetItem();
                childPlant->setText(0, (*plant)[k].name);
                childCompany->addChild(static_cast<QTreeWidgetItem*>(&childPlant);
                QList<cShop> *shop = new QList<cShop>();
                if(!db->get_shop(shop, (*plant)[j].id))
                    continue;
                for(int k=0; k<shop->size(); k++) {
                    QTreeWidgetItem *childShop = new QTreeWidgetItem();
                    childShop->setText(0, (*shop)[k].name);
                    childPlant->addChild(static_cast<QTreeWidgetItem*>(&childShop);
                }
            }
        }
    }
    */
    return false;
}

bool MainWindow::displayModelTree()
{
    qDebug() << "displayModelTree()";
    //delete ui->treeWidgetElement->takeTopLevelItem(0);
    hasDB = false;
    ui->treeWidgetModel->clear();
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetModel);
    root->setText(0, "Models");
    ui->treeWidgetElement->addTopLevelItem(root);

    //QList<QTreeWidgetItem*> modelWdgt;
    db->get_model(&modelWdgt);

    for(int i=0; i<modelWdgt.size(); i++) {
        root->addChild(modelWdgt[i]);
    }
    hasDB = true;
    return false;
}
bool MainWindow::displayElementTree()
{
    qDebug() << "displayElementTree()";
    //delete ui->treeWidgetElement->takeTopLevelItem(0);
    hasDB = false;
    ui->treeWidgetElement->clear();
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetElement);
    root->setText(0, "Elements");
    ui->treeWidgetElement->addTopLevelItem(root);

    for(int i=0; i<db->elements.size(); i++) {
        root->addChild(db->elements[i].get_widget());
    }
    hasDB = true;
    return false;
}


void MainWindow::on_treeWidgetElement_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(hasDB) {
        QString msg = current->whatsThis(0);
        qDebug() << msg;
        if(current->whatsThis(0).contains("Element")) {
            cElementWidget *elm = static_cast<cElementWidget*>(current);
            elm->cParent->printInfo();
        }
        else if(current->whatsThis(0).contains("Point")) {
            cPointWidget *pnt = static_cast<cPointWidget*>(current);
            pnt->cParent->printInfo();
        }
    }
}

void MainWindow::elementRightClickMenu(const QPoint &pos)
{
    hasDB = false;
    QTreeWidget *tree = ui->treeWidgetElement;
    QTreeWidgetItem *item = tree->itemAt(pos);

    qDebug()<<pos<<item->text(0);

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
            delElement(elmWdg);
            for(int i=0; i<elmWdg->cParent->points.size(); i++) {
                int _id = elmWdg->cParent->points[i]->id;
                db->deleteDB("point", _id);
            }
            db->deleteDB("element", elmId);

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
            db->deleteDB("point", pntId);
            delPoint(pntWdg);
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
            newElement();
        }
    }
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

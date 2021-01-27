#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QFileDialog"
#include "QDebug"
#include "cDB.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , hasDB(false)
{
    ui->setupUi(this);
    ui->actionClose_Project->setEnabled(false);
    this->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}


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
    QDir pathDir(currProjPath);
    if(!pathDir.exists())
        pathDir.mkpath(currProjPath);
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

void MainWindow::on_actionECHO_DATABASE_triggered() {
    if(!hasDB)
        qDebug() << "No databases.";
    else {
        /*
        cRoute route;
        db->get_route(1, route);
        route.print();
        */
        displayDBTree();
    }
}

void MainWindow::on_actionClose_Project_triggered() {
    if(hasDB) {
        delete db;
        hasDB = false;
        //delete currProjName;
        //delete currProjPath;
        currProjName = currProjPath = "";
        ui->actionClose_Project->setEnabled(false);
    }
}

void MainWindow::on_actionOpen_triggered() {
    QString hostname = QDir::home().dirName();
    QString path = "C:/Users/"+hostname+"/Documents/VmerProjects/";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), path,
                                                    tr("Project Files (*.sqlite3)"));
    qDebug() << fileName;
    openDB(&fileName, true);
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

    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetDB);
    root->setText(0, (*db_table)[0].name);
    ui->treeWidgetDB->addTopLevelItem(root);

    for(int i=0; i<db_table->size(); i++) {
        QList<cCompany> *company = new QList<cCompany>();
        if(!db->get_company(company, (*db_table)[i].id))
            continue;
        for(int j=0; j<company->size(); j++) {
            QTreeWidgetItem *childCompany = new QTreeWidgetItem();
            childCompany->setText(0, (*company)[j].name);
            root->addChild(childCompany);
            QList<cPlant> *plant = new QList<cPlant>();
            if(!db->get_plant(plant, (*company)[j].id))
                continue;
            for(int k=0; k<plant->size(); k++) {
                QTreeWidgetItem *childPlant = new QTreeWidgetItem();
                childPlant->setText(0, (*plant)[k].name);
                childCompany->addChild(childPlant);
                QList<cShop> *shop = new QList<cShop>();
                if(!db->get_shop(shop, (*plant)[j].id))
                    continue;
                for(int k=0; k<shop->size(); k++) {
                    QTreeWidgetItem *childShop = new QTreeWidgetItem();
                    childShop->setText(0, (*shop)[k].name);
                    childPlant->addChild(childShop);
                }
            }
        }
    }
    return false;
}

bool MainWindow::displayElementTree()
{
    QList<cDBTable> *db_table = new QList<cDBTable>();
    qDebug() << "1. db_table->size() : " << db_table->size();
    if(!db->get_db_table(db_table))
        return false;
    qDebug() << "2. db_table->size() : " << db_table->size();

    if(db_table->size() != 1)
        return false;

    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetDB);
    root->setText(0, (*db_table)[0].name);
    ui->treeWidgetDB->addTopLevelItem(root);

    for(int i=0; i<db_table->size(); i++) {
        QList<cCompany> *company = new QList<cCompany>();
        if(!db->get_company(company, (*db_table)[i].id))
            continue;
        for(int j=0; j<company->size(); j++) {
            QTreeWidgetItem *childCompany = new QTreeWidgetItem();
            childCompany->setText(0, (*company)[j].name);
            root->addChild(childCompany);
            QList<cPlant> *plant = new QList<cPlant>();
            if(!db->get_plant(plant, (*company)[j].id))
                continue;
            for(int k=0; k<plant->size(); k++) {
                QTreeWidgetItem *childPlant = new QTreeWidgetItem();
                childPlant->setText(0, (*plant)[k].name);
                childCompany->addChild(childPlant);
                QList<cShop> *shop = new QList<cShop>();
                if(!db->get_shop(shop, (*plant)[j].id))
                    continue;
                for(int k=0; k<shop->size(); k++) {
                    QTreeWidgetItem *childShop = new QTreeWidgetItem();
                    childShop->setText(0, (*shop)[k].name);
                    childPlant->addChild(childShop);
                }
            }
        }
    }
    return false;
}

void MainWindow::on_actionQUERY_ALL_TABLES_triggered()
{
    if(!hasDB)
        qDebug() << "No Database.";
    else {
        qDebug() << "query_db_table : " << db->query_db_table();
        db->print_db_table();
        qDebug() << "query_element_table : " << db->query_element_table();
        db->print_element_table();
        qDebug() << "query_point_table : " << db->query_point_table();
        db->print_point_table();
        qDebug() << "query_company_table : " << db->query_company_table();
        db->print_company_table();
        qDebug() << "query_plant_table : " << db->query_plant_table();
        db->print_plant_table();
        qDebug() << "query_shop_table : " << db->query_shop_table();
        db->print_shop_table();
        qDebug() << "query_route_table : " << db->query_route_table();
        db->print_route_table();
        qDebug() << "query_models_table : " << db->query_models_table();
        db->print_models_table();
        qDebug() << "query_machine_table : " << db->query_machine_table();
        db->print_machine_table();
        qDebug() << "query_element_in_model_table : " << db->query_element_in_model_table();
        db->print_element_in_model_table();
        qDebug() << "query_machine_in_route_table : " << db->query_machine_in_route_table();
        db->print_machine_in_route_table();
        qDebug() << "query_point_in_route_table : " << db->query_point_in_route_table();
        db->print_point_in_route_table();
    }
}

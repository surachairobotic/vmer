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
#include <QPlainTextEdit>
#include "commonFunction.h"
#include "cElementProperties.h"

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

    ui->treeWidgetRoute->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidgetRoute, &QTreeWidget::customContextMenuRequested, this, &MainWindow::routeRightClickMenu);

    connect(ui->treeWidgetElement, &QTreeWidget::itemChanged, this, [=](QTreeWidgetItem *item, int column) {
        cPointWidget *p = (cPointWidget*)item;
        if(p) {
            qDebug() << "QTreeWidget::itemChanged : col= " << column << ", name: " << p->text(0);
            if(p->whatsThis(0) == "Point" && p->text(0) != p->cParent->name) {
                qDebug() << "p->text(0) :" << p->text(0) << ", p->cParent->name :" << p->cParent->name;
                p->cParent->name = p->text(0);
                db->update("point", p->cParent->id, p->text(0));
            }
        }
        else
            qDebug() << "QTreeWidget::itemChanged : NULL";
    });

    this->setWindowTitle("V-Soft");
    this->setFocus();
    openProject("/home/sugar/Documents/VmerProjects/t8/t8.sqlite3");
}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::openDB(const QString *fname, const QString *fpath, const bool script) {
    qDebug("openDB");
    QString *dbFile = new QString(*fpath+*fname+".sqlite3");
    return openDB(dbFile, script);
}
bool MainWindow::openDB(const QString *fname, const bool script) {
    qDebug("openDB2");
    db = new cDB(*fname);
    hasDB = true;
    ui->actionClose_Project->setEnabled(true);
    if(script)
        db->load_script_file(":/db_scripts/vmer_sqlite_script.sql");
    return true;
}

void MainWindow::on_actionClose_Project_triggered() {
    qDebug("on_actionClose_Project_triggered");
    if(hasDB) {
        delete db;
        hasDB = false;
        //delete currProjName;
        //delete currProjPath;
        currProjName = currProjPath = currProjImage = "";
        ui->treeWidgetDB->clear();
        ui->treeWidgetElement->clear();
        ui->treeWidgetModel->clear();
        ui->treeWidgetRoute->clear();
        ui->actionClose_Project->setEnabled(false);
    }
}

void MainWindow::on_actionOpen_triggered() {
    qDebug("on_actionOpen_triggered");
    QString hostname = QDir::home().dirName();
    QString path = "C:/Users/"+hostname+"/Documents/VmerProjects/";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), path,
                                                    tr("Project Files (*.sqlite3)"));
    openProject(fileName);
}

void MainWindow::openProject(const QString &fname) {
    if(fname != "") {
        qDebug() << fname;
        realProjName = fname.section('/', -2, -2);
        realProjPath = fname.section('/', 0, -2) + "/";
        realProjImage = (realProjPath+"images/");
        openProject(realProjName, realProjPath);
    }
    else
        qDebug("cancel");
}

void MainWindow::openProject(const QString &name, const QString &path, bool db) {
    currProjName = name;
    currProjPath = path.section('/',0,-3) + "/vmer_tmp/" + currProjName + '/';
    currProjImage = (currProjPath+"images/");

    /*
    qDebug() << "currProjName : " << currProjName;
    qDebug() << "currProjPath : " << currProjPath;
    qDebug() << "currProjImage : " << currProjImage;

    qDebug() << "realProjName : " << realProjName;
    qDebug() << "realProjPath : " << realProjPath;
    qDebug() << "realProjImage : " << realProjImage;
    */

    cpDir(realProjPath, currProjPath);

    QString fileName = currProjPath + '/' + currProjName + ".sqlite3";
    openDB(&fileName, db);
    updateDatabase();

    displayDBTree();
    displayElementTree();
    displayModelTree();
    displayRouteTree();
}

bool MainWindow::rmDir(const QString &dirPath) {
    qDebug("rmDir");
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

bool MainWindow::cpDir(const QString &srcPath, const QString &dstPath) {
    qDebug("cpDir");
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
void MainWindow::on_treeWidgetElement_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    qDebug("on_treeWidgetElement_currentItemChanged");
    if(hasDB) {
        if(current == nullptr)
            return ;
        QString msg = current->whatsThis(0);
        QString prev = "NULL";
        if(previous != nullptr)
            prev = previous->whatsThis(0);
        qDebug() << prev << ", " << msg;
        if(current->whatsThis(0).contains("Element")) {
            ui->tabWidgetRight->setCurrentIndex(0);
            cElementWidget *elm = static_cast<cElementWidget*>(current);
            //elm->cParent->printInfo();
            elementGraphics(elm->cParent);
//            ui->plainTextEdit_prop->setPlainText(elm->cParent->desc);
//            QWidget *tabWdgt = ui->properties;

//            QTableWidget *qDrvTab = new QTableWidget();
//            qDrvTab->setRowCount(5);
//            qDrvTab->setColumnCount(2);
            commonFunction::clearLayout(ui->vLayoutPropTab);
//            QPlainTextEdit *plainTxt = new QPlainTextEdit();
//            plainTxt->setPlainText(elm->cParent->desc);
//            ui->vLayoutPropTab->addWidget(plainTxt);
            QWidget *wdg = new QWidget(this);
            cElementProperties *element_prop = new cElementProperties(wdg);
            if(!element_prop->init() || !element_prop->load_json_file("/home/sugar/vmer/element_prop/templates/elements/Electric motor/properties.txt")){
                QTimer::singleShot(0, this, SLOT(close()));
            }
            ui->vLayoutPropTab->addWidget(wdg);
//            connect(ui->pushButtonSaveElementProperties, &QPushButton::clicked, this, [=]() {
//                QString json_str;

//                // create JSON string
//                if( !element_prop->create_json(json_str)){
//                    return;
//                }

//                // save the string to file
//                QFile file( "properties_save.txt" );
//                if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
//                {
//                    QMessageBox::warning(this, tr("Alert")
//                        , QString("Cannot create json file"));
//                    return;
//                }
//                QTextStream stream( &file );
//                stream.setCodec("UTF-8");
//                stream.setGenerateByteOrderMark(true);
//                stream << json_str;
//                file.close();
//                QMessageBox::warning(this, tr("Alert")
//                    , QString("OK"));
//            });
        }
        else if(current->whatsThis(0).contains("Point")) {
            ui->tabWidgetRight->setCurrentIndex(1);
            cPointWidget *pnt = static_cast<cPointWidget*>(current);
            pnt->cParent->printInfo();
            int indxEle = -1;
            for(int i=0; i<db->elements.size(); i++) {
                if(db->elements[i].id == pnt->cParent->element_id) {
                    indxEle = i;
                    break;
                }
            }
            elementGraphics(&db->elements[indxEle]);
            commonFunction::clearLayout(ui->vLayoutPropTab);
            QTableWidget *tableWdgt = new QTableWidget();
            tableWdgt->setColumnCount(4);
            QStringList labels;
            labels << tr("Point Name") << tr("V") << tr("H") << tr("A");
            tableWdgt->setHorizontalHeaderLabels(labels);
            tableWdgt->horizontalHeader()->setStretchLastSection(true);
            QHeaderView *vh=new QHeaderView(Qt::Vertical);
            vh->hide();
            tableWdgt->setVerticalHeader(vh);
            int row = db->elements[indxEle].points.size();
            tableWdgt->setRowCount(row);
            for(int i=0; i<row; i++) {
                QList<QString> lStr = {"p"+db->elements[indxEle].points[i]->name};
                QList<QString> sConfg = commonFunction::pointJson( db->elements[indxEle].points[i]->config );
                lStr += sConfg;
                tableWdgt->insertRow(i);
                for(int j=0; j<lStr.size(); j++) {
                    QTableWidgetItem *itm = new QTableWidgetItem(lStr[j]);
                    itm->setTextAlignment(Qt::AlignCenter);
                    tableWdgt->setItem(i, j, itm);
                }
            }
            ui->vLayoutPropTab->addWidget(tableWdgt);
        }
    }
}


void MainWindow::elementGraphics(const cElement *ele, bool del) {
    qDebug("elementGraphics");
    QString imgName = getElementImagePath(ele);
    img = new QPixmap(imgName);
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

//    int row=ele->points.size();
//    QTableWidget *pntTable = new QTableWidget(row+1, 1);
//    pntTable->setHorizontalHeaderLabels({"Point ID"});
//    for(int i=0; i<row; i++) {
//        QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(ele->points[i]->name));
//        pntTable->setItem(i, 1, newItem);
//    }
//    hLayout->addWidget(pntTable);

    if(del) {
        QLayoutItem* item;
        while ( del && ( item = ui->vLayoutInfoTab->layout()->takeAt( 0 ) ) != NULL ) {
            delete item->widget();
            delete item;
        }
    }

    ui->vLayoutInfoTab->addLayout(hLayout);
}

QString MainWindow::getElementImagePath(const cElement *ele) {
    QString res;
    if(ele->image != "")
        res = currProjPath + "images/user_elements/" + ele->image;
    else
        res = currProjPath + "images/standard_elements/" + ele->std_image;
    qDebug() << res;
    return res;
}


bool MainWindow::newPoint(int element_id) {
    qDebug("newPoint");
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
    //db->delete_point()
}
void MainWindow::delElement(QTreeWidgetItem *item) {
    qDebug() << "delElement : " << item->text(0);
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
            ui->tabWidgetRight->setCurrentIndex(0);
            cElementWidget *elm = static_cast<cElementWidget*>(current);
            //elm->cParent->printInfo();
            elementGraphics(elm->cParent);
            commonFunction::clearLayout(ui->vLayoutPropTab);
            QPlainTextEdit *plainTxt = new QPlainTextEdit();
            plainTxt->setPlainText(elm->cParent->desc);
            ui->vLayoutPropTab->addWidget(plainTxt);
//            ui->plainTextEdit_prop->setPlainText(elm->cParent->desc);
        }
        else if(current->whatsThis(0).contains("Model")) {
            ui->tabWidgetRight->setCurrentIndex(0);
            cModelWidget *mdl = static_cast<cModelWidget*>(current);
            qDebug() << "address of mdl->cParent : " << mdl->cParent;
            mdl->cParent->printInfo("on_treeWidgetModel_currentItemChanged");
            QString msg = QString("Model name : " + mdl->cParent->name);
            bool bStart=true;
            for(int i=0; i<mdl->cParent->elements.size(); i++) {
                elementGraphics(mdl->cParent->elements[i], bStart);
                bStart=false;
                msg += QString("\n--> Element name : " + mdl->cParent->elements[i]->name);
            }
//            ui->plainTextEdit_prop->setPlainText(msg);
            commonFunction::clearLayout(ui->vLayoutPropTab);
            QTableWidget *tableWdgt = new QTableWidget();
            tableWdgt->setColumnCount(1);
            QStringList labels;
            labels << tr("Element Name");
            tableWdgt->setHorizontalHeaderLabels(labels);
            tableWdgt->horizontalHeader()->setStretchLastSection(true);
            QHeaderView *vh=new QHeaderView(Qt::Vertical);
            vh->hide();
            tableWdgt->setVerticalHeader(vh);
            int row = mdl->cParent->elements.size();
            tableWdgt->setRowCount(row);
            for(int i=0; i<row; i++) {
                tableWdgt->insertRow(i);
                QTableWidgetItem *itm = new QTableWidgetItem(mdl->cParent->elements[i]->name);
                itm->setTextAlignment(Qt::AlignCenter);
                tableWdgt->setItem(i, 0, itm);
            }
            ui->vLayoutPropTab->addWidget(tableWdgt);
        }
    }
}

void MainWindow::on_actionSave_Project_triggered() {
    qDebug("newModel");
    if(hasDB)
        cpDir(currProjPath, realProjPath);
}

void MainWindow::on_btn_addModel_clicked()
{
    on_actionNew_Model_Window_triggered();
}


void MainWindow::on_btn_addElement_clicked()
{
    on_actionNew_Element_Window_triggered();
}


void MainWindow::on_treeWidgetRoute_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current == nullptr)
        return ;
    QString msg = QString(current->whatsThis(0) + " : " + current->text(0));
//    qDebug() << current->whatsThis(0) << " : " << current->text(0);
    QList<QString> type;
    if(current->whatsThis(0).contains("DBTable"))
        type = {"Company", "Plant", "Route"};
    else if(current->whatsThis(0).contains("Company"))
        type = {"Plant", "Route"};
    else if(current->whatsThis(0).contains("Plant"))
        type = {"Route"};
    else if(current->whatsThis(0).contains("Route")) {
        ui->tabWidgetRight->setCurrentIndex(0);
        commonFunction::clearLayout(ui->vLayoutInfoTab);
        QPlainTextEdit *plainTxt = new QPlainTextEdit();
        QString desc = ((cRouteWidget*)current)->cParent->desc;
        msg += QString("\nJson : \n"+desc);
        plainTxt->setPlainText(msg);
        ui->vLayoutInfoTab->addWidget(plainTxt);
    }
    QList<int> count;
    for(int i=0; i<type.size(); i++)
        count.append(0);
    msg += showItemInfo(current, &type, &count);
    //ui->plainTextEdit_prop->setPlainText(msg);
    commonFunction::clearLayout(ui->vLayoutPropTab);
    QPlainTextEdit *plainTxt = new QPlainTextEdit();
    plainTxt->setPlainText(msg);
    ui->vLayoutPropTab->addWidget(plainTxt);
}

QString MainWindow::showItemInfo(const QTreeWidgetItem *current, const QList<QString> *type, QList<int> *counts) {
    int s = (*type).size();
    int *count = new int[s];
    for(int i=0; i<s; i++)
        count[i]=0;
    itemRecusiveCount(current, type, count);
    QString res = "";
    for(int i=0; i<(*type).size(); i++) {
        res += QString("\n" + (*type)[i] + " : " + QString::number(count[i]));
        (*counts)[i] = count[i];
    }
    return res;
}

void MainWindow::itemRecusiveCount(const QTreeWidgetItem *current, const QList<QString> *type, int *count) {
    for(int i=0; i<(*type).size(); i++)
        if(current->whatsThis(0).contains((*type)[i])) {
            count[i]++;
            break;
        }
    for(int i=0; i<current->childCount(); i++)
        itemRecusiveCount(current->child(i), type, count);
}

void MainWindow::on_treeWidgetDB_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current == nullptr)
        return ;
    QString msg = QString(current->whatsThis(0) + " : " + current->text(0));
//    qDebug() << current->whatsThis(0) << " : " << current->text(0);
    QList<QString> type;
    bool isMachine=false;
    if(current->whatsThis(0).contains("DBTable"))
        type = {"Company", "Plant", "Shop", "Machine"};
    else if(current->whatsThis(0).contains("Company"))
        type = {"Plant", "Shop", "Machine"};
    else if(current->whatsThis(0).contains("Plant"))
        type = {"Shop", "Machine"};
    else if(current->whatsThis(0).contains("Shop"))
        type = {"Machine"};
    else if(current->whatsThis(0).contains("Machine"))
        isMachine=true;
    QList<int> count;
    for(int i=0; i<type.size(); i++)
        count.append(0);
    msg += showItemInfo(current, &type, &count);
//    ui->plainTextEdit_prop->setPlainText(msg);
    if(isMachine) {
        cMachineWidget *itm = (cMachineWidget*)(current);
        int indx=-1;
        for(int i=0; i<db->models.size(); i++)
            if(db->models[i].id == itm->cParent->model_id) {
                indx=i;
                break;
            }
        if(indx != -1)
            msg += QString("\nModel : "+db->models[indx].name);
        msg += QString("\nSerial number : "+itm->cParent->serial_number);
        ui->tabWidgetRight->setCurrentIndex(1);
    }
    else
        ui->tabWidgetRight->setCurrentIndex(0);

    commonFunction::clearLayout(ui->vLayoutPropTab);
    QPlainTextEdit *plainTxt = new QPlainTextEdit();
    plainTxt->setPlainText(msg);
    ui->vLayoutPropTab->addWidget(plainTxt);

    QList<QList<QString>> data;
    data.append( { "Type name", "Quantity", "Last Data Date" } );
    for(int i=0; i<type.size(); i++)
        data.append( { type[i], QString::number(count[i]), "" } );
    commonFunction::createTable(&data, ui->vLayoutInfoTab);
}

bool MainWindow::checkAddressExist(QTreeWidgetItem *itm) {
    if(itm->whatsThis(0).contains("DBTable")) {
        cDBTableWidget* wdg = static_cast<cDBTableWidget*>(itm);
        qDebug() << "DBTable address : " << wdg->cParent;
        for(int i=0; i<db->dbTables.size(); i++)
            qDebug() << "db->dbTables[" << i << "] : " << &db->dbTables[i];
    }
    else if(itm->whatsThis(0).contains("Company")) {
        cCompanyWidget* wdg = static_cast<cCompanyWidget*>(itm);
        qDebug() << "Company address : " << wdg->cParent;
        for(int i=0; i<db->companies.size(); i++)
            qDebug() << "db->companies[" << i << "] : " << &db->companies[i];
    }
    else if(itm->whatsThis(0).contains("Plant")) {
        cPlantWidget* wdg = static_cast<cPlantWidget*>(itm);
        qDebug() << "Plant address : " << wdg->cParent;
        for(int i=0; i<db->plants.size(); i++)
            qDebug() << "db->plants[" << i << "] : " << &db->plants[i];
    }
    return false;
}

void MainWindow::on_tabWidgetLeft_currentChanged(int index)
{
    commonFunction::clearLayout(ui->vLayoutInfoTab);
    commonFunction::clearLayout(ui->vLayoutPropTab);
}

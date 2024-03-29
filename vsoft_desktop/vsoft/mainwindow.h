#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cNewProjectDialog.h"
#include "cNewElementWindow.h"
#include "cNewModelWindow.h"
#include "cNewShopWindow.h"
#include "cNewRouteWindow.h"
#include "cNewMachineWindow.h"
#include <cDB.h>
#include <QPixmap>
#include <QGraphicsView>
#include <QTableWidget>
#include <QHBoxLayout>
#include "cReportWindow.h"

class cReport;
class cHTTPRequestGUI;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include<iostream>
using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool openDB(const QString *file, const QString *path, const bool script);
    bool openDB(const QString *fname, const bool script);

    Ui::MainWindow *ui;

private slots:
    void on_actionNew_Project_triggered();
    void newprojDialog_accepted(const QString fname, const QString fpath);
    void newprojDialog_rejected();

    void on_actionECHO_DATABASE_triggered();
    void on_actionClose_Project_triggered();
    void on_actionOpen_triggered();

    void on_actionQUERY_ALL_TABLES_triggered();

    void on_treeWidgetElement_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void dbRightClickMenu(const QPoint &pos);
    void modelRightClickMenu(const QPoint &pos);
    void elementRightClickMenu(const QPoint &pos);
    void routeRightClickMenu(const QPoint &pos);

    void on_treeWidgetModel_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_actionNew_Element_Window_triggered();
    void on_actionNew_Model_Window_triggered();
    void on_actionNew_Shop_Window_triggered(int plantId);
    void on_actionNew_Route_Window_triggered(int plantId);
    void on_actionNew_Machine_Window_triggered(int shpId);

    void newElementMain(QString name, QString stdImg, QString userImg, QString descElement, QList<QString> pntName, QList<QString> pntDesc);
    void newModelMain(QString name, QList<int> eleIds);
    void newShopMain(int _plantId, QString name, QList<int> mdlIds);
    void newMachineMain(QString name, int mdlId, int shpId, QString serial, QString desc);
    void newRouteMain();

    void on_actionSave_Project_triggered();

    void on_btn_addModel_clicked();

    void on_btn_addElement_clicked();

    void on_treeWidgetRoute_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_treeWidgetDB_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_tabWidgetLeft_currentChanged(int index);

    void on_actionReport_triggered();

    void on_actionGraph_triggered();

private:
    void cNewProjectDialogTerminate();
    bool displayDBTree();
    bool displayElementTree();
    bool displayModelTree();
    bool displayRouteTree();
    void updateDatabase();

    QTreeWidgetItem* newShop(int _plantId, QString name, QList<int> mdlIds);
    QTreeWidgetItem* newShop(int _plantId, QString name);
    bool newModel();
    bool newModel(QString name, QList<int> eleIds);
    bool newElement(QString name, QString stdImg, QString userImg, QString descElement, QList<QString> pntName, QList<QString> pntDesc);
    bool newPoint(int element_id);
    bool newPoint2(int element_id);
    void delPoint(QTreeWidgetItem *item);
    void delElement(QTreeWidgetItem *item);

    void elementGraphics(const cElement *ele, bool del=true);
    QString getElementImagePath(const cElement *ele);
//    bool rmDir(const QString &dirPath);
//    bool cpDir(const QString &srcPath, const QString &dstPath);
    void openProject(const QString &fname);
    void openProject(const QString &name, const QString &path, bool db=false);

    void itemRecusiveCount(const QTreeWidgetItem *current, const QList<QString> *type, int *count);
    QString showItemInfo(const QTreeWidgetItem *current, const QList<QString> *type, QList<int> *counts);
    bool checkAddressExist(QTreeWidgetItem *itm);

    void showReportWin(void);

    cNewProjectDialog *newprojDialog;
    cNewElementWindow *newEleWin;
    cNewModelWindow *newMdlWin;
    cNewShopWindow *newShpWin;
    cNewRouteWindow *newRouteWin;
    cNewMachineWindow* newMachineWin;
    cDB *db;
    QString currProjName, currProjPath, currProjImage, realProjName, realProjPath, realProjImage;
    bool hasDB;
    QAction *newAct, *delAct, *jsonAct, *renmAct;
    QList<QTreeWidgetItem*> modelWdgt, dbWdgt, eleWdgt, routeWdgt;
    QPixmap *img;
    QHBoxLayout *hLayout;
    QGraphicsView *gView;
    QGraphicsScene *scene;
    int newShopCount = 1;

    cReport *report;
    friend class cReport;
    //cReportWindow *reportWin;

    cHTTPRequestGUI *request;

    void testPointer(int *k);
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cNewProjectDialog.h"
#include "cNewElementWindow.h"
#include "cNewModelWindow.h"
#include "cNewShopWindow.h"
#include "cNewRouteWindow.h"
#include <cDB.h>
#include <QPixmap>
#include <QGraphicsView>
#include <QTableWidget>
#include <QHBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool openDB(const QString *file, const QString *path, const bool script);
    bool openDB(const QString *fname, const bool script);

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

    void newElementMain(QString name, QString stdImg, QString userImg, QList<QString> pntNames);
    void newModelMain(QString name, QList<int> eleIds);
    void newShopMain(int _plantId, QString name, QList<int> mdlIds);

    void on_actionSave_Project_triggered();

private:
    void cNewProjectDialogTerminate();
    bool displayDBTree();
    bool displayElementTree();
    bool displayModelTree();
    bool displayRouteTree();
    void updateDatabase();

    bool newShop(int _plantId, QString name, QList<int> mdlIds);
    bool newModel();
    bool newModel(QString name, QList<int> eleIds);
    bool newElement(QString name, QString stdImg, QString userImg, QList<QString> pntName);
    bool newPoint(int element_id);
    void delPoint(QTreeWidgetItem *item);
    void delElement(QTreeWidgetItem *item);

    void elementGraphics(const cElement *ele, bool del=true);
    QString getElementImagePath(const cElement *ele);
    bool rmDir(const QString &dirPath);
    bool cpDir(const QString &srcPath, const QString &dstPath);
    void openProject(const QString &name, const QString &path, bool db=false);


    Ui::MainWindow *ui;
    cNewProjectDialog *newprojDialog;
    cNewElementWindow *newEleWin;
    cNewModelWindow *newMdlWin;
    cNewShopWindow *newShpWin;
    cNewRouteWindow *newRouteWin;
    cDB *db;
    QString currProjName, currProjPath, currProjImage, realProjName, realProjPath, realProjImage;
    bool hasDB;
    QAction *newAct, *delAct, *jsonAct;
    QList<QTreeWidgetItem*> modelWdgt, dbWdgt, eleWdgt, routeWdgt;
    QPixmap *img;
    QHBoxLayout *hLayout;
    QGraphicsView *gView;
    QGraphicsScene *scene;
};
#endif // MAINWINDOW_H

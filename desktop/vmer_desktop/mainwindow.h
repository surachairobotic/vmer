#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cNewProjectDialog.h"
#include "cNewElementWindow.h"
#include "cNewModelWindow.h"
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

    void on_treeWidgetModel_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_actionNew_Element_Window_triggered();
    void on_actionNew_Model_Window_triggered();

private:
    void cNewProjectDialogTerminate();
    bool displayDBTree();
    bool displayElementTree();
    bool displayModelTree();
    void updateDatabase();

    bool newModel();
    bool newElement();
    bool newPoint(int element_id);
    void delPoint(QTreeWidgetItem *item);
    void delElement(QTreeWidgetItem *item);

    void elementGraphics(const cElement *ele, bool del=true);


    Ui::MainWindow *ui;
    cNewProjectDialog *newprojDialog;
    cNewElementWindow *newEleWin;
    cNewModelWindow *newMdlWin;
    cDB *db;
    QString currProjName, currProjPath, currProjImage;
    bool hasDB;
    QAction *newAct, *delAct;
    QList<QTreeWidgetItem*> modelWdgt, dbWdgt, eleWdgt;
    QPixmap *img;
    QHBoxLayout *hLayout;
    QGraphicsView *gView;
    QGraphicsScene *scene;
};
#endif // MAINWINDOW_H

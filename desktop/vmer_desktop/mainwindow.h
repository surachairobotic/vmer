#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cNewProjectDialog.h"
#include <cDB.h>

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

    void modelRightClickMenu(const QPoint &pos);
    void elementRightClickMenu(const QPoint &pos);

private:
    void cNewProjectDialogTerminate();
    bool displayDBTree();
    bool displayElementTree();
    bool displayModelTree();
    void updateDatabase();

    bool newElement();
    bool newPoint(int element_id);
    void delPoint(QTreeWidgetItem *item);
    void delElement(QTreeWidgetItem *item);


    Ui::MainWindow *ui;
    cNewProjectDialog *newprojDialog;
    cDB *db;
    QString currProjName, currProjPath, currProjImage;
    bool hasDB;
    QAction *newAct, *delAct;
    QList<QTreeWidgetItem*> modelWdgt;
};
#endif // MAINWINDOW_H

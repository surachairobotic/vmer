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

private:
    void cNewProjectDialogTerminate();
    bool displayDBTree();
    bool displayElementTree();

    Ui::MainWindow *ui;
    cNewProjectDialog *newprojDialog;
    cDB *db;
    QString currProjName, currProjPath;
    bool hasDB;
};
#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets/QMessageBox>
#include "cHTTPRequestGUI.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , request(NULL)
{
    ui->setupUi(this);
    request = new cHTTPRequestGUI(this);
    if( !request->init() ){
        QMessageBox::warning(this, tr("Alert"), tr("Cannot initilize Request"));
        QTimer::singleShot(0, this, SLOT(close()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if( request )
        delete request;
}

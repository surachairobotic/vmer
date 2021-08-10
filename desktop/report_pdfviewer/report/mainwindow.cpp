#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cReport.h"
#include <QtWidgets/QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , report(NULL)
{
    ui->setupUi(this);

    report = new cReport(this);
    if( report->set_template_file("../../report_pdfviewer/report/report_template.html") ){
        QApplication::exit(0);
    }
    if(!report->init()){
        QMessageBox::warning(this, tr("Alert"), tr("Cannot initilize Report"));
        QApplication::exit(0);
    }
}

MainWindow::~MainWindow()
{
    if( report )
        delete report;
    delete ui;
}


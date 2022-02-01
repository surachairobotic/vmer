#include "cReportWindow.h"
#include "ui_cReportWindow.h"

cReportWindow::cReportWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cReportWindow)
{
    ui->setupUi(this);
}

cReportWindow::~cReportWindow()
{
    delete ui;
}

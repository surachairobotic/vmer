#ifndef CREPORTWINDOW_H
#define CREPORTWINDOW_H

#include <QDialog>
//#include "cReport.h"

namespace Ui {
class cReportWindow;
}

//class cReport;

class cReportWindow : public QDialog
{
    Q_OBJECT

public:
    explicit cReportWindow(QWidget *parent = nullptr);
    ~cReportWindow();

    Ui::cReportWindow *ui;
//private:
//    friend class cReport;
};

#endif // CREPORTWINDOW_H

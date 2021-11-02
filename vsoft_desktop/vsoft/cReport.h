#ifndef CREPORT_H
#define CREPORT_H

#include "mainwindow.h"
#include "cReportWindow.h"
#include <QDir>

class QPdfWidget;

class cReport : public QObject{
    Q_OBJECT
public:
    cReport(QWidget *_parent);
    ~cReport();

    bool init();
    bool set_template_file(const QString& path);
    bool create_pdf(const QString& path);
    bool show_pdf(const QString& path);

    bool my_show();
    cReportWindow *reportWin;

protected slots:
    void slot_show_pdf();
private:
    bool get_html(QString &html);

    QString template_path;

    QWidget *parent;
    QDir tmp_dir;
    QPdfWidget *pdf;


    //friend class cReportWindow;
};

#endif // CREPORT_H

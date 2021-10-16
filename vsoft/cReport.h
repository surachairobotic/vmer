#ifndef CREPORT_H
#define CREPORT_H

#include "mainwindow.h"
#include <QDir>

class QPdfWidget;

class cReport : public QObject{
    Q_OBJECT
public:
    cReport(MainWindow *_mainwindow);
    ~cReport();

    bool init();
    bool set_template_file(const QString& path);
    bool create_pdf(const QString& path);
    bool show_pdf(const QString& path);
protected slots:
    void slot_show_pdf();
private:
    bool get_html(QString &html);

    QString template_path;

    MainWindow *mainwindow;
    QDir tmp_dir;
    QPdfWidget *pdf;
};

#endif // CREPORT_H

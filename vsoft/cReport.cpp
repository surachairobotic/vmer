#include "cReport.h"
#include "ui_mainwindow.h"
#include <QTextBrowser>
#include <QtCore>
#include <QDebug>
#include <QtWidgets/QMessageBox>

#include <QAction>
#include <QToolBar>
#include <QIcon>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFileInfo>
#include "QPdfWidget"

#include "mainwindow.h"
#include "wkhtmltox/pdf.h"
#include "wkhtmltox/image.h"

cReport::cReport(MainWindow *_mainwindow) : mainwindow(_mainwindow),pdf(NULL){

}

cReport::~cReport(){
    wkhtmltopdf_deinit();
    wkhtmltoimage_deinit();
}

bool cReport::init(){
    wkhtmltopdf_init(false);
    wkhtmltoimage_init(false);

    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    qDebug() << "Tmp dir : " << path;
    tmp_dir.setPath(path);
    if (!tmp_dir.exists()) {
        if (!tmp_dir.mkpath(path)) {
            QMessageBox::warning(mainwindow, tr("Alert")
                , QString("Cannot create tmp directory : ") + tmp_dir.absolutePath());
            return false;
        }
    }
    pdf = new QPdfWidget();
    mainwindow->ui->verticalLayoutReport->addWidget(pdf);

    connect( pdf, &QPdfWidget::pdfDocumentLoaded, this, [=](){
        qDebug() << "pdf load fin";
    });



    connect(mainwindow->ui->pushButtonRefreshReport, &QPushButton::clicked, this, &cReport::slot_show_pdf);
    return true;
}

bool cReport::set_template_file(const QString& path) {
    QFileInfo check_file(path);
    if (check_file.exists() && check_file.isFile()) {
        template_path = path;
        return true;
    }
    else {
        QMessageBox::warning(mainwindow, tr("Alert")
            , QString("Report template file does not exist : ") + path);
        qWarning() << "File not exist : " << path;
        return false;
    }
}


bool cReport::get_html(QString& html) {
    const QString title = "Title 01";
    const QDateTime now = QDateTime::currentDateTime();

    QFile file(template_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(mainwindow, tr("Alert")
            , QString("Cannot open report template file: ") + template_path);
        return false;
    }
    html = file.readAll();
    file.close();
    html.replace("%title%", title);
    html.replace("%datetime%", now.toString());
    return true;
}


bool cReport::create_pdf(const QString &path) {

    wkhtmltopdf_global_settings* gs;
    wkhtmltopdf_object_settings* os;
    wkhtmltopdf_converter* c;


    gs = wkhtmltopdf_create_global_settings();
    wkhtmltopdf_set_global_setting(gs, "out", path.toStdString().c_str());
    wkhtmltopdf_set_global_setting(gs, "load.cookieJar", "myjar.jar");

    os = wkhtmltopdf_create_object_settings();
//    wkhtmltopdf_set_object_setting(os, "page", template_path.toStdString().c_str());

    c = wkhtmltopdf_create_converter(gs);
    /*
    wkhtmltopdf_set_progress_changed_callback(c, progress_changed);
    wkhtmltopdf_set_phase_changed_callback(c, phase_changed);
    wkhtmltopdf_set_error_callback(c, error);
    wkhtmltopdf_set_warning_callback(c, warning);*/

    QString html;
    bool ret = false;
    if (get_html(html)){
        wkhtmltopdf_add_object(c, os, html.toStdString().c_str());
        if (!wkhtmltopdf_convert(c)) {
            QMessageBox::warning(mainwindow, tr("Alert")
                , QString("Cannot create report pdf : err=") + QString::number(wkhtmltopdf_http_error_code(c)));
        }
        else {
            if (wkhtmltopdf_http_error_code(c) != 0) {
                qDebug() << "httpErrorCode: " << wkhtmltopdf_http_error_code(c);
            }
            ret = true;
        }
    }
    wkhtmltopdf_destroy_converter(c);
    return ret;
}

void cReport::slot_show_pdf() {
    const QString tmp_pdf = tmp_dir.absoluteFilePath("tmp_report.pdf");
    if(!create_pdf(tmp_pdf)){
        return;
    }
    if (!pdf->loadFile(tmp_pdf)) {
        QMessageBox::warning(mainwindow, tr("Alert")
            , QString("Cannot load pdf file : ") + tmp_pdf);
        return;
    }
    // Update window title with the file name
    //QFileInfo fi(tmp_pdf);
    //setWindowTitle(fi.fileName());
}

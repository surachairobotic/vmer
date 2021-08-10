#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cElementProperties.h"
#include <QtWidgets/QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    element_prop = new cElementProperties(this);
    if(!element_prop->init() || !element_prop->load_json_file("../element_prop/templates/elements/Electric motor/properties.txt")){
        QTimer::singleShot(0, this, SLOT(close()));
    }

    connect(ui->pushButtonSaveElementProperties, &QPushButton::clicked, this, [=]() {
        QString json_str;

        // create JSON string
        if( !element_prop->create_json(json_str)){
            return;
        }

        // save the string to file
        QFile file( "properties_save.txt" );
        if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QMessageBox::warning(this, tr("Alert")
                , QString("Cannot create json file"));
            return;
        }
        QTextStream stream( &file );
        stream.setCodec("UTF-8");
        stream.setGenerateByteOrderMark(true);
        stream << json_str;
        file.close();
        QMessageBox::warning(this, tr("Alert")
            , QString("OK"));
    });

}

MainWindow::~MainWindow()
{
    if( element_prop )
            delete element_prop;
    delete ui;
}


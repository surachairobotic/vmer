#include "cNewProjectDialog.h"
#include "ui_cNewProjectDialog.h"
#include "QDebug"
#include "QFileDialog"
#include "QDialogButtonBox"
#include "QMessageBox"

cNewProjectDialog::cNewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cNewProjectDialog)
{
    ui->setupUi(this);
}

cNewProjectDialog::~cNewProjectDialog() {
    delete ui;
}

void cNewProjectDialog::on_btnBrowse_clicked() {
    QString dir;
    if(ui->lineEdit_fpath->text() == "") {
        QString hostname = QDir::home().dirName();
        QString path = "C:/Users/"+hostname+"/Documents/VmerProjects/";
        dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     path,
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    }
    else {
        dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     ui->lineEdit_fpath->text(),
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    }
    if(dir.size()>0) {
        if(dir[dir.size()-1] != '/')
            dir += '/';
        ui->lineEdit_fpath->setText(dir);
    }
}

void cNewProjectDialog::on_buttonBox_clicked(QAbstractButton *button) {
    QDialogButtonBox::StandardButton stdButton = ui->buttonBox->standardButton(button);
    if(stdButton == QDialogButtonBox::Ok) {
        qDebug() << "stdButton == QDialogButtonBox::Ok";
        QDir pathDir(ui->lineEdit_fpath->text());
        if(ui->lineEdit_fname->text() == "" ||
           ui->lineEdit_fpath->text() == "" ||
           !pathDir.exists() ||
           pathDir.exists(ui->lineEdit_fname->text())) {
           QMessageBox::warning(this, tr("Warning !"),
                                      tr("The file name or path is incorrect."),
                                      QMessageBox::Ok);
        }
        else {
            emit accepted(ui->lineEdit_fname->text(), ui->lineEdit_fpath->text()+ui->lineEdit_fname->text()+'/');
        }
    }
    else if(stdButton == QDialogButtonBox::Cancel) {
        qDebug() << "stdButton == QDialogButtonBox::Cancel";
        emit rejected();
    }
}

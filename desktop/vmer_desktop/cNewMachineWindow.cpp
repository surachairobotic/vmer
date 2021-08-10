#include "cNewMachineWindow.h"
#include "ui_cNewMachineWindow.h"
#include <QAbstractButton>
#include <QMessageBox>

cNewMachineWindow::cNewMachineWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cNewMachineWindow)
{
    ui->setupUi(this);
}

cNewMachineWindow::~cNewMachineWindow()
{
    delete ui;
}

void cNewMachineWindow::initData() {
    for(int i=0; i<db->models.size(); i++) {
        ui->comboBox->addItem(db->models[i].name);
    }
}

void cNewMachineWindow::on_buttonBox_clicked(QAbstractButton *button) {
    if((QPushButton *)button == ui->buttonBox->button(QDialogButtonBox::Ok)) {
        qDebug() << "ok";
        if(ui->textEdit_machineName->toPlainText() != "" && ui->textEdit_serial->toPlainText() != "") {
            QString name = ui->textEdit_machineName->toPlainText();
            QString serial = ui->textEdit_serial->toPlainText();
            int mdlId = db->models[ui->comboBox->currentIndex()].id;
            emit newMachineAccepted(name, mdlId, shpId, serial, "");
            done(Accepted);
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Error !");
            msgBox.exec();
        }
    }
    else {
        qDebug() << "cancel";
        done(Rejected);
    }
}

void cNewMachineWindow::accept() {
//    done(Accepted);
}

void cNewMachineWindow::reject() {
//    done(Rejected);
}


#ifndef CNEWELEMENTWINDOW_H
#define CNEWELEMENTWINDOW_H

#include <QDialog>

namespace Ui {
class cNewElementWindow;
}

class cNewElementWindow : public QDialog
{
    Q_OBJECT

public:
    explicit cNewElementWindow(QWidget *parent = nullptr);
    ~cNewElementWindow();



private slots:
    void on_btn_addPoint_clicked();

    void on_btn_addTemplate_clicked();

    void on_buttonBox_accepted();

    void on_btn_editImage_clicked();

private:
    Ui::cNewElementWindow *ui;
};

#endif // CNEWELEMENTWINDOW_H
/*
#include <QAbstractButton>

public:
    explicit cNewProjectDialog(QWidget *parent = nullptr);
    ~cNewProjectDialog();

signals:
    void accepted(QString fname, QString fpath);
    void rejected();

private slots:
    void on_btnBrowse_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::cNewProjectDialog *ui;
};

#endif // CNEWPROJECTDIALOG_H
*/

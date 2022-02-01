#ifndef CNEWPROJECTDIALOG_H
#define CNEWPROJECTDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class cNewProjectDialog;
}

class cNewProjectDialog : public QDialog
{
    Q_OBJECT

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

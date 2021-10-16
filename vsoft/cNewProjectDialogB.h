#ifndef CNEWPROJECTDIALOGB_H
#define CNEWPROJECTDIALOGB_H

#include <QDialog>

namespace Ui {
class cNewProjectDialogB;
}

class cNewProjectDialogB : public QDialog
{
    Q_OBJECT

public:
    explicit cNewProjectDialogB(QWidget *parent = nullptr);
    ~cNewProjectDialogB();

private:
    Ui::cNewProjectDialogB *ui;
};

#endif // CNEWPROJECTDIALOGB_H

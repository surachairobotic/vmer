#ifndef CNEWPROJECT_H
#define CNEWPROJECT_H

#include <QWidget>

namespace Ui {
class cNewProject;
}

class cNewProject : public QWidget
{
    Q_OBJECT

public:
    explicit cNewProject(QWidget *parent = nullptr);
    ~cNewProject();

private:
    Ui::cNewProject *ui;
    QWidget *myparent;
};

#endif // CNEWPROJECT_H

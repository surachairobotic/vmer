#ifndef CNEWPROJECT_H
#define CNEWPROJECT_H

#include <QMainWindow>

namespace Ui {
class cNewProject;
}

class cNewProject : public QMainWindow
{
    Q_OBJECT

public:
    explicit cNewProject(QWidget *parent = nullptr);
    ~cNewProject();

private:
    Ui::cNewProject *ui;
};

#endif // CNEWPROJECT_H

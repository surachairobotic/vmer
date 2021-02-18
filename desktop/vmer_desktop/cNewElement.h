#ifndef CNEWELEMENT_H
#define CNEWELEMENT_H

#include <QDialog>

namespace Ui {
class cNewElement;
}

class cNewElement : public QDialog
{
    Q_OBJECT

public:
    explicit cNewElement(QWidget *parent = nullptr);
    ~cNewElement();

private:
    Ui::cNewElement *ui;
};

#endif // CNEWELEMENT_H

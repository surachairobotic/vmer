#ifndef CNEWELEMENTWINDOW_H
#define CNEWELEMENTWINDOW_H

#include <QDialog>
#include <QGraphicsView>

namespace Ui {
class cNewElementWindow;
}

class cNewElementWindow : public QDialog
{
    Q_OBJECT

public:
    explicit cNewElementWindow(QWidget *parent = nullptr);
    ~cNewElementWindow();

    QString currProjName, currProjPath, currProjImage;

Q_SIGNALS:
    void newElementAccepted(QString name, QString stdImg, QString userImg, QList<QString> pntName);
    //void setMainwindow(MainWindow *main);

private slots:
    void on_btn_addPoint_clicked();
    void on_buttonBox_accepted();
    void on_btn_stdImage_clicked();
    void on_btn_userImage_clicked();

private:
    void add_graphics(QString fname, QGraphicsView *gView);
    QString img_browse(QString path, QGraphicsView *gView);

    Ui::cNewElementWindow *ui;
    QString eleName, stdImg, userImg;
    QList<QString> pntName;
    //MainWindow *main;

};

#endif // CNEWELEMENTWINDOW_H

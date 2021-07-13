#include "cNewRouteWindow.h"
#include "ui_cNewRouteWindow.h"
#include <QMessageBox>

cNewRouteWindow::cNewRouteWindow(QWidget *parent, int _plantId) :
    QDialog(parent),
    ui(new Ui::cNewRouteWindow),
    plantId(_plantId)
{
    ui->setupUi(this);
}

cNewRouteWindow::~cNewRouteWindow() {
    delete ui;
}

void cNewRouteWindow::createPlantTable() {
    ui->treeWidget_left->clear();
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidget_left);
    root->setExpanded(true);
    db->get_plants(&plntWdgt, true);
    for(int i=0; i<plntWdgt.size(); i++)
        root->addChild(plntWdgt[i]);
    ui->treeWidget_left->addTopLevelItem(root);
}

void cNewRouteWindow::on_treeWidget_left_itemChanged(QTreeWidgetItem *item, int column)
{
//    qDebug() << "on_treeWidget_left_itemChanged : column=" << column;
//    qDebug() << "ColumnCount : " << item->columnCount();
//    qDebug() << "Text(0) : " << item->text(0);
//    qDebug() << "childCount() : " << item->childCount();
//    checkStateProcess(item, column);
    /*
    if(item->checkState(column)) {
        qDebug() << "ColumnCount : " << item->columnCount();
        qDebug() << "Text(0) : " << item->text(0);
        qDebug() << "childCount() : " << item->childCount();
        bool isBlank = true;
        for(int i=0; i<item->childCount(); i++)
            if(item->child(i)->checkState(0)) {
                isBlank = true;
                break;
            }
        if(isBlank) {
            for(int i=0; i<item->childCount(); i++)
                item->child(i)->setCheckState(0, QT::C);
        }
    }
    else {
        qDebug() << "Uncheck";
    }
    */
}


void cNewRouteWindow::on_treeWidget_left_itemClicked(QTreeWidgetItem *item, int column) {
    checkStateProcess(item, column);
}

void cNewRouteWindow::checkStateProcess(QTreeWidgetItem *item, int column) {
    qDebug() << "checkStateProcess : column=" << column;
    qDebug() << "ColumnCount : " << item->columnCount();
    qDebug() << "Text(0) : " << item->text(0);
    qDebug() << "childCount() : " << item->childCount();

    if(item->checkState(0) == Qt::Checked) {
        //item->setCheckState(0, Qt::Unchecked);
        checkAllChild(item, Qt::Unchecked);
        //checkAllParent(item, Qt::PartiallyChecked);
    }
    else {
        //item->setCheckState(0, Qt::Checked);
        checkAllChild(item, Qt::Checked);
    }
}

void cNewRouteWindow::checkAllChild(QTreeWidgetItem *item, Qt::CheckState state) {
    if(item->childCount() > 0)
        for(int i=0; i<item->childCount(); i++)
            checkAllChild(item->child(i), state);
    item->setCheckState(0, state);
    checkAllChildNotDiff(item);
}

void cNewRouteWindow::checkAllChildNotDiff(QTreeWidgetItem *item) {
    if(item->parent()) {
        bool notDiff = true;
        Qt::CheckState chkState = item->checkState(0);
        for(int i=0; i<item->parent()->childCount(); i++)
            if(item->parent()->child(i)->checkState(0) != chkState) {
                notDiff = false;
                break;
            }
        if(notDiff)
            item->parent()->setCheckState(0, chkState);
        else
            item->parent()->setCheckState(0, Qt::PartiallyChecked);
        checkAllChildNotDiff(item->parent());
    }
}

void cNewRouteWindow::checkAllParent(QTreeWidgetItem *item, Qt::CheckState state) {
    if(item->parent()) {
        item->parent()->setCheckState(0, state);
        checkAllParent(item->parent(), state);
    }
}

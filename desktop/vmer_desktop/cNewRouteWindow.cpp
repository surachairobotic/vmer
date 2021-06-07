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

#include "commonFunction.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>

QList<QString> commonFunction::pointJson(QString msg) {
    QJsonDocument jsonResponse = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QList<int> vha = {jsonObject["V"].toInt(), jsonObject["H"].toInt(), jsonObject["A"].toInt()};
    QList<QString> res;
    for(int i=0; i<vha.size(); i++)
        res.append( (vha[i]) ? "T" : "F" );
    return res;
}

void commonFunction::createTable(const QList<QList<QString>> *data, QLayout *layout) {
    commonFunction::clearLayout(layout);
    if((*data).size() < 1)
        return;
    if((*data)[0].size() < 1)
        return;
    QTableWidget *tableWdgt = new QTableWidget();
    int col = (*data)[0].size();
    tableWdgt->setColumnCount(col);
    QStringList *labels = new QStringList((*data)[0]);
    tableWdgt->setHorizontalHeaderLabels(*labels);
    tableWdgt->horizontalHeader()->setStretchLastSection(true);
    QHeaderView *vh=new QHeaderView(Qt::Vertical);
    vh->hide();
    tableWdgt->setVerticalHeader(vh);
    int row = (*data).size();
    tableWdgt->setRowCount(row);
    for(int i=1; i<row; i++) {
        tableWdgt->insertRow(i-1);
        for(int j=0; j<col; j++) {
            QTableWidgetItem *itm = new QTableWidgetItem((*data)[i][j]);
            itm->setTextAlignment(Qt::AlignCenter);
            tableWdgt->setItem(i-1, j, itm);
        }
    }
    layout->addWidget(tableWdgt);
}

void commonFunction::clearLayout(QLayout* layout, bool deleteWidgets) {
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (deleteWidgets) {
            if (QWidget* widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            commonFunction::clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}

QAction* commonFunction::actionHandle(const QList<QString> *txt, QWidget *parent, const QList<QString> *status, QPoint pos) {
    if(!txt || !status || (*txt).size() < 1 || (*status).size()!=(*txt).size())
        return nullptr;

    QMenu menu(parent);
    for(int i=0; i<(*txt).size(); i++) {
        QAction *act = new QAction((*txt)[i], parent);
        act->setStatusTip((*status)[i]);
        menu.addAction(act);
    }
    return menu.exec(pos);
}
QList<QString> commonFunction::menu2status(QList<QString> *txt, QString parentName) {
    QList<QString> res;
    for(int i=0; i<(*txt).size(); i++)
        res.append(parentName+" : "+(*txt)[i]);
    return res;
}

void commonFunction::setAllChildExpanded(QTreeWidgetItem *itm) {
    itm->setExpanded(true);
    for(int i=0; i<itm->childCount(); i++)
        setAllChildExpanded(itm->child(i));
}


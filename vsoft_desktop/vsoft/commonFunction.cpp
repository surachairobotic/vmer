#include "commonFunction.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>
#include <QDir>


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
        //qDebug() << "50 : " << item->
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

void commonFunction::setWidgetCheckState(QTreeWidgetItem *itm, QString msg) {
    if(itm->whatsThis(0).contains(msg))
        itm->setCheckState(0, Qt::Unchecked);
    for(int i=0; i<itm->childCount(); i++)
        setWidgetCheckState(itm->child(i), msg);
}

bool commonFunction::rmDir(const QString &dirPath) {
    qDebug("rmDir");
    QDir dir(dirPath);
    if (!dir.exists())
        return true;
    foreach(const QFileInfo &info, dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        if (info.isDir()) {
            if (!rmDir(info.filePath()))
                return false;
        } else {
            if (!dir.remove(info.fileName()))
                return false;
        }
    }
    QDir parentDir(QFileInfo(dirPath).path());
    return parentDir.rmdir(QFileInfo(dirPath).fileName());
}

bool commonFunction::cpDir(const QString &srcPath, const QString &dstPath) {
    qDebug("cpDir");
    rmDir(dstPath);
    QDir parentDstDir(QFileInfo(dstPath).path());
    //QString strDstPath = QFileInfo(dstPath).fileName();
    if (!parentDstDir.mkpath(dstPath))
        return false;

    QDir srcDir(srcPath);
    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        int chk=0;
        if (info.isDir()) {
            if (!cpDir(srcItemPath, dstItemPath)) {
                return false;
            }
            chk=1;
        } else if (info.isFile()) {
            if (!QFile::copy(srcItemPath, dstItemPath)) {
                return false;
            }
            chk=2;
        } else {
            qDebug() << "Unhandled item" << info.filePath() << "in cpDir";
            chk=3;
        }
        qDebug() << "chk = " << chk;
    }
    return true;
}

bool commonFunction::isFile(QString name) {
    return name.contains('.');
}

void commonFunction::mkpath(const QString path) {
    QDir *pathDir = new QDir(path);
    if(!pathDir->exists())
        pathDir->mkpath(path);
}

/*
    for(int i=0;i<2;i++){
        QTreeWidgetItem *treeItem_shop = new QTreeWidgetItem();
        treeItem_shop->setText(0, QString("Machine shop ") + QString::number(i+1));
        treeItem_db->addChild(treeItem_shop);
        for(int j=0;j<2;j++){
            QTreeWidgetItem *treeItem_route = new QTreeWidgetItem();
            treeItem_route->setText(0, QString("Route ") + QString::number(i*2+j+1));
            treeItem_route->setCheckState(0, Qt::Unchecked);
            treeItem_shop->addChild(treeItem_route);

    QList<QTreeWidgetItem*> *routeWdgt = new QList<QTreeWidgetItem*>();
    db->get_routes(routeWdgt);

    mainwindow->ui->treeWidgetRoute_2->addTopLevelItem((*routeWdgt)[0]);
    //(*routeWdgt)[0]->parent() = mainwindow->ui->treeWidgetRoute_2;

    qDebug() << "create_route_tree";
    for(int i=0; i<routeWdgt->size(); i++) {
        qDebug() << (*routeWdgt)[i]->text(0);
        for(int j=0; j<(*routeWdgt)[i]->childCount(); j++) {
            QTreeWidgetItem *jj = (*routeWdgt)[i]->child(j);
            qDebug() << jj->text(0);
        }
    }
}

*/

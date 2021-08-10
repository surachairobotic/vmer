#include "cNewRouteWindow.h"
#include "ui_cNewRouteWindow.h"
#include <QMessageBox>
#include "commonFunction.h"

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
    db->get_plants(&plntWdgt, true);
    for(int i=0; i<plntWdgt.size(); i++) {
        ui->treeWidget_left->addTopLevelItem(plntWdgt[i]);
        plntWdgt[i]->setExpanded(true);
        commonFunction::setAllChildExpanded(plntWdgt[i]);
    }
    ui->treeWidget_left->setColumnCount(1);
    QStringList labels;
    labels << tr("Select machines");
    ui->treeWidget_left->setHeaderLabels(labels);
    ui->treeWidget_left->header()->setDefaultAlignment(Qt::AlignCenter);
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
        checkAllChild(item, Qt::Unchecked);
    }
    else {
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

void cNewRouteWindow::accept() {
//    done(Accepted);
}

void cNewRouteWindow::reject() {
    done(Rejected);
}

void cNewRouteWindow::on_buttonBox_clicked(QAbstractButton *button) {
    if((QPushButton *)button == ui->buttonBox->button(QDialogButtonBox::Ok)) {
        qDebug() << "ok";
        bool isCheck=false;
        for(int i=0; i<ui->treeWidget_left->topLevelItemCount(); i++) {
            if(ui->treeWidget_left->topLevelItem(i)->checkState(0) == Qt::Checked ||
               ui->treeWidget_left->topLevelItem(i)->checkState(0) == Qt::PartiallyChecked) {
                isCheck=true;
                break;
            }
        }
        if(ui->textEdit_routeName->toPlainText() != "" && isCheck) {
            int id = commonFunction::findMaxId(&db->routes)+1;
            QString name = ui->textEdit_routeName->toPlainText();
            QJsonDocument json = tree2json(ui->treeWidget_left, name, id);
            cRoute *route = new cRoute(id, plantId, name, json.toJson());
            db->insert(route);
            qDebug() << "------- json ---------";
            qDebug() << json;
            qDebug() << "------- json end ---------";
            saveJson(json, "/home/sugar/Documents/VmerProjects/test.json");
            done(Accepted);
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Error !");
            msgBox.exec();
        }
    }
    else {
        qDebug() << "cancel";
    }
}

void cNewRouteWindow::saveJson(QJsonDocument document, QString fileName) {
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

QJsonDocument cNewRouteWindow::tree2json(const QTreeWidget *wdgt, const QString routeName, const int routeId) {
    QJsonObject root;
    root.insert("routeName", routeName);
    root.insert("routeId", routeId);
    //root.insert("routeId", plantId);
    //db->plants
    QString plntName = commonFunction::findNameById(&(db->plants), plantId);
    int plntIndx = commonFunction::findIndexById(&db->plants, plantId);
    int companyId = db->plants[plntIndx].company_id;
    QString companyName = commonFunction::findNameById(&db->companies, companyId);
    int companyIndx = commonFunction::findIndexById(&db->companies, companyId);
    int dbId = db->companies[companyIndx].db_id;
    QString dbName = commonFunction::findNameById(&db->dbTables, dbId);

    root.insert("dbName", dbName);
    root.insert("dbId", QJsonValue::fromVariant(dbId));
    root.insert("companyName", companyName);
    root.insert("companyId", QJsonValue::fromVariant(companyId));
    root.insert("plantName", plntName);
    root.insert("plantId", QJsonValue::fromVariant(plantId));

    QJsonArray arr;
    for(int i=0; i<wdgt->topLevelItemCount(); i++) {
        QJsonObject *obj = recursiveJsonItem(wdgt->topLevelItem(i));
        if(obj)
            arr.push_back(*obj);
    }
    root.insert("machineList", arr);
    QJsonDocument jsonDoc(root);
    return jsonDoc;
}

QString cNewRouteWindow::recursiveTreeItem(const QTreeWidgetItem *wdgt) {
    QString res;
    if(wdgt->checkState(0) != Qt::Unchecked) {
        QList<QString> type = {"Plant", "Shop", "Route"};
        for(int i=0; i<type.size(); i++)
            if(wdgt->whatsThis(0).contains(type[i]))
                res = QString(type[i]+" : "+wdgt->text(0));
        for(int i=0; i<wdgt->childCount(); i++)
            res += QString("\n"+recursiveTreeItem(wdgt->child(i)));
    }
    return res;
}
QJsonObject* cNewRouteWindow::recursiveJsonItem(const QTreeWidgetItem *wdgt) {
    QJsonObject *res = nullptr;
    if(wdgt->checkState(0) != Qt::Unchecked) {
        QList<QString> type =      {"Plant", "Shop" , "Machine"};
        QList<QString> typeChild = {"Shop" , "Machine", "Error"};
        int typeIndx=-1;
        for(int i=0; i<type.size(); i++) {
            if(wdgt->whatsThis(0).contains(type[i])) {
                res = new QJsonObject();
                QString x = type[i]+"Name";
                x[0] = x[0].toLower();
                res->insert(x, wdgt->text(0));
                typeIndx=i;
            }
        }
        if(res == nullptr)
            return res;
        if(wdgt->childCount() == 1) {
            QJsonObject *child = recursiveJsonItem(wdgt->child(0));
            if(child != nullptr)
                res->insert(typeChild[typeIndx], *child);
        }
        else if(wdgt->childCount() > 1) {
            QJsonArray arr;
            for(int i=0; i<wdgt->childCount(); i++) {
                QTreeWidgetItem *itm = wdgt->child(i);
                QJsonObject *obj = recursiveJsonItem(itm);
                if(obj) {
                    QString x = type[i]+"Name";
                    x[0] = x[0].toLower();
                    arr.push_back(*obj);
                }
            }
            res->insert(typeChild[typeIndx], arr);
        }
    }
    return res;
}






















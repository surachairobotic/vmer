#include "cElementProperties.h"
#include "ui_mainwindow.h"
#include <QTextBrowser>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QComboBox>

cElementProperties::cElementProperties(QWidget *parent) : parent(parent){

}

cElementProperties::~cElementProperties(){

}

bool cElementProperties::init(){
    //auto table = this->mainwindow->ui->tableWidgetElementProperties;
    table = new QTableWidget(parent);
    table->setColumnCount(2);
    table->setShowGrid(true);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setHorizontalHeaderLabels(QStringList() << "Property" << "Value");
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);

    return true;
}

bool cElementProperties::load_json(const QString &json_str){
    //auto table = this->mainwindow->ui->tableWidgetElementProperties;
    table->clearContents();

    json = QJsonDocument::fromJson(json_str.toStdString().c_str());
    if (json.isNull()) {
//        QMessageBox::warning(mainwindow, tr("Alert")
//            , QString("Invalid JSON data"));
        qDebug() << "Invalid JSON data";
        return false;
    }
    try{
        int n_prop = 0;
        foreach(const QJsonValue & r, json["properties"].toArray()){
            QJsonObject prop = r.toObject();
            QString type = prop["type"].toString();

            QString prop_name = prop["name"].toString();
            table->insertRow(n_prop);
            QTableWidgetItem *item = new QTableWidgetItem(prop_name);
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            table->setItem(n_prop, 0, item);
            if( type=="select" ){
                QComboBox* combo = new QComboBox;
                QString cur_value = prop["current_value"].toString();
                bool b_cur_value = false;
                combo->setProperty("type",QVariant("select"));
                foreach(const QJsonValue & r, prop["values"].toArray()){
                    QJsonObject o = r.toObject();
                    QString name = o["name"].toString()
                        , value = o["value"].toString();
                    combo->addItem(name, QVariant(value));
                    if( !b_cur_value && value==cur_value ){
                        combo->setCurrentIndex(combo->count()-1);
                        b_cur_value = true;
                    }
                }
                if( !b_cur_value ){
                    throw QString("Invalid current value of property '" + prop_name + "': " + cur_value);
                }
                table->setCellWidget( n_prop, 1, combo);
            }
            else if( type=="integer" ){
                auto line = new QLineEdit();
                auto vld = new QIntValidator(
                            //prop["min_value"].toInt(),
                            //prop["max_value"].toInt(),
                            line);
                line->setValidator( vld );
                line->setText(QString::number(prop["current_value"].toInt()));
                line->setProperty("type",QVariant(type));
                const int min = prop["min_value"].toInt(), max = prop["max_value"].toInt();
                connect(line, &QLineEdit::textChanged, this, [=]() {
                    int n = line->text().toInt();
                    if( n<min )
                        line->setText( QString::number(min) );
                    else if( n>max )
                        line->setText( QString::number(max) );
                });

                table->setCellWidget(n_prop, 1, line);
            }
            else if( type=="float" ){
                auto line = new QLineEdit();
                auto vld = new QDoubleValidator(
                //            prop["min_value"].toDouble(),
                //            prop["max_value"].toDouble(),
                //            5,
                            line);
                vld->setNotation(QDoubleValidator::StandardNotation);
                line->setValidator( vld );
                line->setProperty("type",QVariant(type));
                const double min = prop["min_value"].toDouble(), max = prop["max_value"].toDouble();
                connect(line, &QLineEdit::textChanged, this, [=]() {
                    double n = line->text().toDouble();
                    if( n<min )
                        line->setText( QString::number(min) );
                    else if( n>max )
                        line->setText( QString::number(max) );
                });

                line->setText(QString::number(prop["current_value"].toDouble()));
                table->setCellWidget(n_prop, 1, line);
            }
            else if( type=="string" ){
                auto line = new QLineEdit();
                int max_len = prop["max_length"].toInt();
                if( max_len>0 )
                    line->setMaxLength(max_len);
                line->setText(prop["current_value"].toString());
                line->setProperty("type",QVariant(type));
                table->setCellWidget(n_prop, 1, line);
            }
            else{
                throw QString("Invalid property type of '" + prop_name + "': " + type);
            }
            n_prop++;
        }
        return true;
    }
    catch(const QString msg){
//        QMessageBox::warning(mainwindow, tr("Alert")
//            , msg);
        qDebug() << msg;
    }
    catch(...){
//        QMessageBox::warning(mainwindow, tr("Alert")
//            , QString("Unknown exception thrown"));
        qDebug() << "Unknown exception thrown";
    }
    table->clearContents();
    return false;
}

bool cElementProperties::load_json_file(const QString &path){
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
//        QMessageBox::warning(mainwindow, tr("Alert")
//            , QString("Cannot open json file : ") + path);
        qDebug() << QString("Cannot open json file : ") + path;
        return false;
    }
    QString json = file.readAll();
    file.close();
    return load_json(json);
}

bool cElementProperties::create_json(QString &json_str){
//    auto table = this->mainwindow->ui->tableWidgetElementProperties;
    if( table->rowCount()==0 ){
//        QMessageBox::warning(mainwindow, tr("Alert")
//            , QString("No property"));
        qDebug() << "No property";
        return false;
    }
    QJsonObject jo = json.object();
    QJsonArray props = jo["properties"].toArray();
    for(int i=0;i<table->rowCount();i++){
        QWidget *widget = table->cellWidget(i, 1);
        QJsonObject prop = props[i].toObject();
        QString type = widget->property("type").toString();

        // check if property name matched
        if( prop["name"].toString()!=table->item(i,0)->text() ){
//            QMessageBox::warning(mainwindow, tr("Alert")
//                , QString("Property name does not match with JSON doc : ")
//                +prop["name"].toString()
//                +" / "+table->item(i,0)->text());
            qDebug() << QString("Property name does not match with JSON doc : ")
                    +prop["name"].toString()
                    +" / "+table->item(i,0)->text();
            return false;
        }
        if( type=="select" ){
            prop["current_value"] = static_cast<QComboBox*>(widget)->currentData().toString();
        }
        else if( type=="integer" ){
            prop["current_value"] = static_cast<QLineEdit*>(widget)->text().toInt();
        }
        else if( type=="float" ){
            prop["current_value"] = static_cast<QLineEdit*>(widget)->text().toDouble();
        }
        else if( type=="string" ){
            prop["current_value"] = static_cast<QLineEdit*>(widget)->text();
        }
        props[i] = prop;
    }
    jo["properties"] = props;
    json_str = QJsonDocument(jo).toJson(QJsonDocument::Compact).toStdString().c_str();
    return true;
}

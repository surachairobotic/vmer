#include "cHTTPRequestGUI.h"
#include "cListWidgetItemDevice.h"
#include <QtWidgets/QMessageBox>
#include <QTreeWidget>
#include <QListWidget>

const QString ROUTE_DIR("./route/");
const QString DATA_DIR("./data/");

cHTTPRequestGUI::cHTTPRequestGUI(MainWindow *_mainwindow)
    : mainwindow(_mainwindow)
    , progress_dialog(NULL)
{

}

cHTTPRequestGUI::~cHTTPRequestGUI()
{

}

bool cHTTPRequestGUI::init(){
    if(!request.init()){
        return false;
    }

    // discover V-MERs and show route data in V-MERs
    connect(mainwindow->ui->pushButtonRefreshDevice, &QPushButton::clicked, this, [=]() {
        mainwindow->ui->listWidgetDevice->clear();
        request.discover_devices();
    });
    connect( mainwindow->ui->listWidgetDevice, &QListWidget::itemChanged, this, &cHTTPRequestGUI::slot_device_changed);
    connect( &request, &cHTTPRequest::signal_get_route_list, this, &cHTTPRequestGUI::slot_get_route_list);
    connect( &request, &cHTTPRequest::signal_device_found, this, &cHTTPRequestGUI::slot_device_found);
    request.discover_devices();


    // show all route data in PC
    create_route_tree();

    // upload route data from PC to V-MER
    connect(mainwindow->ui->pushButtonUploadRoutes, &QPushButton::clicked, this, &cHTTPRequestGUI::upload_routes);
    connect( &request, &cHTTPRequest::signal_progress_upload_routes, this, &cHTTPRequestGUI::slot_progress_upload_routes);

    // remove route data in V-MER
    connect(mainwindow->ui->pushButtonRemoveRoutes, &QPushButton::clicked, this, &cHTTPRequestGUI::remove_routes);
    connect( &request, &cHTTPRequest::signal_remove_routes, this, &cHTTPRequestGUI::slot_remove_routes);

    // tab
    connect(mainwindow->ui->tabWidgetDevice, &QTabWidget::currentChanged, this, [=](int){
        auto *item_device = mainwindow->ui->listWidgetDevice->currentItem();
        if( item_device )
            slot_device_changed(item_device);
    });

    // download measurement data from V-MER to PC
    connect(mainwindow->ui->pushButtonDownloadData, &QPushButton::clicked, this, &cHTTPRequestGUI::download_data);
    connect(mainwindow->ui->pushButtonRefreshData, &QPushButton::clicked, this, [=](){
        cListWidgetItemDevice *item_device = static_cast<cListWidgetItemDevice*>(mainwindow->ui->listWidgetDevice->currentItem());
        mainwindow->ui->listWidgetData->clear();
        request.get_data_list(item_device->get_ip_address());
    });
    connect( &request, &cHTTPRequest::signal_get_data_list, this, &cHTTPRequestGUI::slot_get_data_list);
    connect( &request, &cHTTPRequest::signal_progress_download_data, this, &cHTTPRequestGUI::slot_progress_download_data);
    connect(mainwindow->ui->listWidgetData, &QListWidget::itemChanged, this, [=](QListWidgetItem* item){
        mainwindow->ui->listWidgetData->blockSignals(true);
        if( item->text()=="Select All" ){
            const Qt::CheckState check = item->checkState();
            for(int i=1;i<mainwindow->ui->listWidgetData->count();i++){
                mainwindow->ui->listWidgetData->item(i)->setCheckState(check);
            }
        }
        else{
            const Qt::CheckState check = item->checkState();
            bool b_all = true;
            for(int i=1;i<mainwindow->ui->listWidgetData->count();i++){
                if( mainwindow->ui->listWidgetData->item(i)->checkState()!=check ){
                    b_all = false;
                    break;
                }
            }
            mainwindow->ui->listWidgetData->item(0)->setCheckState(b_all ? check : Qt::Unchecked);
        }
        mainwindow->ui->listWidgetData->blockSignals(false);
    });
    return true;
}

void cHTTPRequestGUI::slot_device_found(bool b_error, const tDeviceInfo device){
    // do nothing if the device did not respond properly
    if(b_error){
        //QMessageBox::warning( mainwindow, tr("Alert"), tr("Cannot get device list"));
        return;
    }
    cListWidgetItemDevice *item_device = new cListWidgetItemDevice(mainwindow->ui->listWidgetDevice
        , device );
    item_device->setText(device.name);
    if( mainwindow->ui->listWidgetDevice->count()==1 ){
        mainwindow->ui->listWidgetDevice->setCurrentRow(0);
    }
}


void cHTTPRequestGUI::slot_device_changed(QListWidgetItem* item){
    cListWidgetItemDevice *item_device = static_cast<cListWidgetItemDevice*>(item);
    auto tab = mainwindow->ui->tabWidgetDevice;
    QString tab_name = tab->tabText(tab->currentIndex());
    qDebug() << "device changed : name = " << item_device->get_name() << ", ip = " << item_device->get_ip_address();
    if( tab_name=="Route" ){
        mainwindow->ui->listWidgetDeviceRoute->clear();
        request.get_route_list(item_device->get_ip_address());
    }
    else if( tab_name=="Measurement Data" ){
        mainwindow->ui->listWidgetData->clear();
        request.get_data_list(item_device->get_ip_address());
    }
    else{
        qWarning() << "Unknown tab name : " << tab_name;
    }
}


void cHTTPRequestGUI::slot_get_route_list(bool b_error, const QList<QString> route_names){
    qDebug() << "slot_get_route_list()";
    if( b_error ){
        QMessageBox::warning( mainwindow, tr("Alert"), tr("Cannot get route list from V-MER"));
        return;
    }
    set_device_route_names(route_names);
}

void cHTTPRequestGUI::create_route_tree(){
    mainwindow->ui->treeWidgetRoute->clear();
    QTreeWidgetItem *treeItem_db = new QTreeWidgetItem(mainwindow->ui->treeWidgetRoute);
    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem_db->setText(0, "DB 01");

    for(int i=0;i<2;i++){
        QTreeWidgetItem *treeItem_shop = new QTreeWidgetItem();
        treeItem_shop->setText(0, QString("Machine shop ") + QString::number(i+1));
        treeItem_db->addChild(treeItem_shop);
        for(int j=0;j<2;j++){
            QTreeWidgetItem *treeItem_route = new QTreeWidgetItem();
            treeItem_route->setText(0, QString("Route ") + QString::number(i*2+j+1));
            treeItem_route->setCheckState(0, Qt::Unchecked);
            treeItem_shop->addChild(treeItem_route);
        }
    }
}

void cHTTPRequestGUI::upload_routes(){
    qDebug() << "upload_routes()";
    cListWidgetItemDevice *item_device = static_cast<cListWidgetItemDevice*>(mainwindow->ui->listWidgetDevice->currentItem());
    if(!item_device){
        QMessageBox::warning(mainwindow, tr("Alert"), tr("Please select V-MER before uploading routes"));
        return;
    }

    QList<QString> route_dirs;
    for( int i = 0; i < mainwindow->ui->treeWidgetRoute->topLevelItemCount(); ++i )
    {
       QTreeWidgetItem *item_db = mainwindow->ui->treeWidgetRoute->topLevelItem( i );
       for( int j = 0; j < item_db->childCount(); ++j ){
           QTreeWidgetItem *item_shop = item_db->child( j );
           for( int k = 0; k < item_shop->childCount(); ++k ){
               QTreeWidgetItem *item_route = item_shop->child( k );
               if( item_route->checkState(0)==Qt::Checked ){
                   route_dirs.append(ROUTE_DIR + item_route->text(0));
                   qDebug() << " Route : " << item_route->text(0);
               }
           }
       }
    }
    if( route_dirs.isEmpty() ){
        QMessageBox::warning(mainwindow, tr("Alert"), tr("No route selected"));
        return;
    }


    progress_dialog = new QProgressDialog("Task in progress.", "Cancel", 0, 100);
    progress_dialog->setWindowModality(Qt::ApplicationModal);
    progress_dialog->setValue(0);
    progress_dialog->show();
    connect(progress_dialog, &QProgressDialog::canceled, this, [=](){
        request.cancel_task();
    });

    if( !request.upload_routes(item_device->get_ip_address(), route_dirs) ){
        QMessageBox::warning(mainwindow, tr("Alert"), QString("Cannot create route data"));
        progress_dialog->setValue(100);
        return;
    }
}

void cHTTPRequestGUI::set_device_route_names(const QList<QString> &route_names){
    mainwindow->ui->listWidgetDeviceRoute->clear();
    foreach(const QString &route, route_names){
        QListWidgetItem *item = new QListWidgetItem(mainwindow->ui->listWidgetDeviceRoute);
        item->setText(route);
        item->setCheckState(Qt::Unchecked);
    }
}

void cHTTPRequestGUI::slot_progress_upload_routes(bool b_error, const float progress, const QList<QString> route_names){
    qDebug() << "progress : " << progress;
    if( b_error ){
        QMessageBox::warning(mainwindow, tr("Alert"), QString("Cannot upload routes to V-MER"));
        progress_dialog->setValue(100);
        return;
    }
    progress_dialog->setValue(progress*100.0);
    if( progress==1.0 ){
        set_device_route_names(route_names);
    }
}


void cHTTPRequestGUI::remove_routes(){
    qDebug() << "remove_routes()";
    cListWidgetItemDevice *item_device = static_cast<cListWidgetItemDevice*>(mainwindow->ui->listWidgetDevice->currentItem());
    if(!item_device){
        QMessageBox::warning(mainwindow, tr("Alert"), tr("Please select V-MER before removing routes"));
        return;
    }

    QList<QString> route_names;
    for( int i = 0; i < mainwindow->ui->listWidgetDeviceRoute->count(); ++i )
    {
       QListWidgetItem *item = mainwindow->ui->listWidgetDeviceRoute->item( i );
       if( item->checkState()==Qt::Checked ){
           route_names.append(item->text());
           qDebug() << " Route : " << item->text();
       }
    }
    if( route_names.isEmpty() ){
        QMessageBox::warning(mainwindow, tr("Alert"), tr("No route selected"));
        return;
    }
    if( !request.remove_routes(item_device->get_ip_address(), route_names) ){
        QMessageBox::warning(mainwindow, tr("Alert"), QString("Cannot remove route data"));
    }
}

void cHTTPRequestGUI::slot_remove_routes(bool b_error, const QList<QString> route_names){
    if( b_error ){
        QMessageBox::warning(mainwindow, tr("Alert"), QString("Cannot remove route data in V-MER"));
        return;
    }
    set_device_route_names(route_names);
}


void cHTTPRequestGUI::slot_get_data_list(bool b_error, const QList<QString> data_names){
    qDebug() << "slot_get_data_list()";
    if( b_error ){
        QMessageBox::warning( mainwindow, tr("Alert"), tr("Cannot get data list from V-MER"));
        return;
    }

    mainwindow->ui->listWidgetData->clear();
    if( data_names.isEmpty()){
        return;
    }
    QListWidgetItem *item = new QListWidgetItem(mainwindow->ui->listWidgetData);
    item->setText("Select All");
    item->setCheckState(Qt::Unchecked);
    foreach(const QString &data, data_names){
        QListWidgetItem *item = new QListWidgetItem(mainwindow->ui->listWidgetData);
        item->setText(data);
        item->setCheckState(Qt::Unchecked);
    }
}


void cHTTPRequestGUI::slot_progress_download_data(bool b_error, const float progress){
    qDebug() << "progress : " << progress;
    if( b_error ){
        QMessageBox::warning(mainwindow, tr("Alert"), QString("Cannot download data from V-MER"));
        progress_dialog->setValue(100);
        return;
    }
    if( progress==1.0 ){
        mainwindow->ui->listWidgetData->clear();
        cListWidgetItemDevice *item_device = static_cast<cListWidgetItemDevice*>(mainwindow->ui->listWidgetDevice->currentItem());
        if(!item_device)
            QMessageBox::warning(mainwindow, tr("Alert"), tr("Cannot get data list since V-MER iss not selected"));
        else{
            QTimer::singleShot(400, this, [=](){
                request.get_data_list(item_device->get_ip_address());
                progress_dialog->setValue(progress*100.0);
            });
            return;
        }
    }
    progress_dialog->setValue(progress*100.0);
}


void cHTTPRequestGUI::download_data(){
    qDebug() << "download_data()";
    cListWidgetItemDevice *item_device = static_cast<cListWidgetItemDevice*>(mainwindow->ui->listWidgetDevice->currentItem());
    if(!item_device){
        QMessageBox::warning(mainwindow, tr("Alert"), tr("Please select V-MER before downloading data"));
        return;
    }

    QList<QString> data_names;
    for( int i = 1; i < mainwindow->ui->listWidgetData->count(); ++i )
    {
       QListWidgetItem *item = mainwindow->ui->listWidgetData->item( i );
       if( item->checkState()==Qt::Checked ){
           data_names.append(item->text());
           qDebug() << " Data : " << item->text();
       }
    }
    if( data_names.isEmpty() ){
        QMessageBox::warning(mainwindow, tr("Alert"), tr("No data selected"));
        return;
    }

    progress_dialog = new QProgressDialog("Task in progress.", "Cancel", 0, 100);
    progress_dialog->setWindowModality(Qt::ApplicationModal);
    progress_dialog->setValue(0);
    progress_dialog->show();
    connect(progress_dialog, &QProgressDialog::canceled, this, [=](){
        request.cancel_task();
    });
    if( !request.download_data(item_device->get_ip_address(), data_names, DATA_DIR) ){
        QMessageBox::warning(mainwindow, tr("Alert"), QString("Cannot download data"));
        progress_dialog->setValue(100);
        return;
    }
}

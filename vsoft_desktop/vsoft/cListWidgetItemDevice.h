#ifndef CLISTWIDGETITEMDEVICE_H
#define CLISTWIDGETITEMDEVICE_H

#include <QListWidget>
#include "cHTTPRequest.h"

class cListWidgetItemDevice : public QListWidgetItem {
public:
    cListWidgetItemDevice(QListWidget *parent) : QListWidgetItem(parent){}
    cListWidgetItemDevice(QListWidget *parent, const tDeviceInfo &_device) : QListWidgetItem(parent)
        , device(_device){
    }

    const inline QString &get_name(){ return device.name; }
    const inline QString &get_ip_address(){ return device.ip_address; }

private:
    tDeviceInfo device;
};

#endif // CLISTWIDGETITEMDEVICE_H

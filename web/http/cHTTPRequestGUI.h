#ifndef CHTTPREQUESTGUI_H
#define CHTTPREQUESTGUI_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cHTTPRequest.h"
#include <QtWidgets/QProgressDialog>

class cHTTPRequestGUI : public QObject{
    Q_OBJECT
public:
    cHTTPRequestGUI(MainWindow *_mainwindow);
    ~cHTTPRequestGUI();

    bool init();
private slots:
    // emitted when a device found after calling discover_devices()
    void slot_device_found(bool b_error, const tDeviceInfo device);

    // emitted when a device was selected
    void slot_device_changed(QListWidgetItem* item);

    // emitted when got route list after calling get_route_list()
    void slot_get_route_list(bool b_error, const QList<QString> route_names);

    // emitted while uploading routes after calling upload_route()
    // progress = 0~1
    void slot_progress_upload_routes(bool b_error, const float progress, const QList<QString> route_list);

    // emitted when route was removed after calling remove_route()
    void slot_remove_routes(bool b_error, const QList<QString> route_names );

    // emitted when got route list after calling get_route_list()
    void slot_get_data_list(bool b_error, const QList<QString> data_names);

    // emitted while downloading data after calling download_data()
    // progress = 0~1
    void slot_progress_download_data(bool b_error, const float progress);

private:
    void create_route_tree();
    void upload_routes();
    void remove_routes();
    void set_device_route_names(const QList<QString> &route_names);
    void download_data();

    cHTTPRequest request;
    MainWindow *mainwindow;
    QProgressDialog *progress_dialog;
};

#endif // CHTTPREQUESTGUI_H

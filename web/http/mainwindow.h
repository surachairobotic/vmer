#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class cHTTPRequestGUI;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
/*
private slots:
    // emitted when a device found after calling discover_devices()
    void slot_device_found(bool b_error, const tDeviceInfo device);

    // emitted while uploading routes after calling upload_route()
    // progress = 0~1
    void slot_progress_upload_route(bool b_error, const float progress);


    // emitted while downloading data after calling download_data()
    // progress = 0~1
    void slot_progress_download_data(bool b_error, const float progress);

    // emitted when got route list after calling get_route_list()
    void slot_get_route_list(bool b_error, const QJsonDocument json);

    // emitted when route was removed after calling remove_route()
    void slot_remove_route(bool b_error, const QString route_name );
*/
private:
    Ui::MainWindow *ui;

    void create_route_tree();
    void upload_routes();

    cHTTPRequestGUI *request;

    friend class cHTTPRequestGUI;
};
#endif // MAINWINDOW_H

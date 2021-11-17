#pragma once

#include <QtCore>
#include <QtNetwork/QUdpSocket>


class QNetworkReply;

struct tDeviceInfo {
	QString ip_address
        , id
        , name;
};

class cHTTPRequest : public QObject {
	Q_OBJECT
public:
	cHTTPRequest();
	~cHTTPRequest();

	// initialize
	bool init();

	// discover devices on the same network
	void discover_devices();

    // get measurement data list from vmer
    bool get_data_list(const QString& ip_address);

    // download measurement data from vmer and save to "data_dir"
    bool download_data(const QString &ip_address, const QList<QString> &data_names, const QString& data_dir);

	// upload route data to vmer
	// all files in "route_dir" folder will be uploaded to vmer
    bool upload_routes(const QString& ip_address, const QList<QString> &route_dirs);

    // get route list from vmer
	bool get_route_list(const QString& ip_address);

	// remove route
	// use "*" to remove all routes
    bool remove_routes(const QString& ip_address, const QList<QString> &route_names);

	// get devices found on the network
	inline const QList<tDeviceInfo>& get_devices() {
		return devices;
	};

    bool cancel_task();


signals:
    // emitted when a device found after calling discover_devices()
    void signal_device_found(bool b_error, const tDeviceInfo device=tDeviceInfo());

    // emitted while uploading routes after calling upload_route()
    // progress = 0~1
    void signal_progress_upload_routes(bool b_error, const float progress=1.0, const QList<QString> route_names=QList<QString>());

    // emitted when got route list after calling get_route_list()
    void signal_get_data_list(bool b_error, const QList<QString> data_names=QList<QString>());

    // emitted while downloading data after calling download_data()
    // progress = 0~1
    void signal_progress_download_data(bool b_error, const float progress=1.0);

    // emitted when got route list after calling get_route_list()
    void signal_get_route_list(bool b_error, const QList<QString> route_names=QList<QString>());

    // emitted when route was removed after calling remove_route()
    void signal_remove_routes(bool b_error, const QList<QString> route_names=QList<QString>());

protected slots:
	void devices_found();

protected:
	bool extract(const QString& file_path, const QString &dst);

	QFile download_file, upload_file;
	QUdpSocket* udpSocket;
	QList<tDeviceInfo> devices;
	QDir tmp_dir;
    QNetworkReply *reply;
};

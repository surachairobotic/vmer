#include "cHTTPRequest.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QHttpPart>
#include <QtWidgets/QMessageBox>
#include <QRandomGenerator>

// for ziplib
#ifdef _MSC_VER
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include <fstream>
#include "ziplib/ZipFile.h"
#include "ziplib/streams/memstream.h"
#include "ziplib/methods/Bzip2Method.h"

const int PORT = 8085   // tcp port on vmer
    , PORT_UDP = 8086;  // udp port on vmer

QString create_random_string(const int len);

cHTTPRequest::cHTTPRequest() :udpSocket(NULL),reply(NULL) { }


bool cHTTPRequest::init(){
    // create dir to keep temporary files
    // C:/Users/<USER>/AppData/Roaming/<APPNAME>
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    tmp_dir.setPath(path);
    if (!tmp_dir.exists()) {
        if (!tmp_dir.mkpath(path)) {
            qWarning() << "init : cannot create tmp path : " << path;
            return false;
        }
    }

    udpSocket = new QUdpSocket(this);
    connect(udpSocket, &QUdpSocket::readyRead, this, &cHTTPRequest::devices_found);
    return true;
}

cHTTPRequest::~cHTTPRequest() {
    if (udpSocket) {
        delete udpSocket;
    }
}


bool cHTTPRequest::get_data_list(const QString& ip_address) {
    auto manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished, this
        , [=](QNetworkReply* reply) {
            if (reply->error()) {
                qWarning() << reply->errorString();
                emit signal_get_data_list( true );
                return;
            }
            std::string s = reply->readAll().toStdString();
            qDebug() << "reply : " << s.c_str();
            QJsonDocument json = QJsonDocument::fromJson(s.c_str());
            if (json.isNull()) {
                qWarning() << "Invalid response from vmer : " << s.c_str();
                emit signal_get_data_list( true );
            }
            else if (json["result"].toString() == "success") {
                QList<QString> data_names;
                foreach(const QJsonValue & r, json["data_names"].toArray()){
                    qDebug() << " Data : " << r.toString();
                    data_names.append(r.toString());
                }
                emit signal_get_data_list( false, data_names );
            }
            else {
                qWarning() << "Error msg : " << json["msg"];
                emit signal_get_data_list( true );
            }
        });

    QString url = QString("http://") + ip_address + ":"
        + QString::number(PORT) + "/api/get_data_list";
    qDebug() << "url : " << url;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    manager->get(request);
    return true;
}


bool cHTTPRequest::download_data(const QString& ip_address, const QList<QString> &data_names, const QString& data_dir) {
    auto manager = new QNetworkAccessManager();

    QString url_data_names;
    foreach( const QString &data_name, data_names){
        if( url_data_names.length()>0 )
            url_data_names+= ",";
        url_data_names+= "\""+data_name+"\"";
    }

    QString url = QString("http://") + ip_address + ":"
        + QString::number(PORT) + "/api/download_data?data={\"data_names\":[" + url_data_names + "]}";
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    reply = manager->get(request);
    qDebug() << "download_data";

    QString zip_path = tmp_dir.absoluteFilePath("data.zip");
    download_file.setFileName(zip_path);
    download_file.remove();
    if (!download_file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open file : " + zip_path;
        return false;
    }
    connect(reply, &QNetworkReply::downloadProgress, this, [=](qint64 ist, qint64 max) {
        qDebug() << " download progress : " << ist << " / " << max;
        download_file.write(reply->read(reply->bytesAvailable()));
        if( ist!=max )
            emit signal_progress_download_data( false, float(ist)/max );
    });
    connect(reply, &QNetworkReply::finished, this, [=]() {
        qDebug() << " download fin";
        download_file.close();
        if( extract(download_file.fileName(), data_dir) )
            emit signal_progress_download_data( false, 1.0 );
        else{
            emit signal_progress_download_data( true );
        }
        download_file.remove();
        if( this->reply ){
            //this->reply->deleteLater();
            this->reply = NULL;
        }
    });
    connect(reply, &QNetworkReply::errorOccurred, this, [=](QNetworkReply::NetworkError e) {
        qWarning() << "Network error : " << e;
        download_file.close();
        download_file.remove();
        emit signal_progress_download_data( true );
        if( this->reply ){
//            this->reply->deleteLater();
            this->reply = NULL;
        }
    });
    return true;
}


bool add_zip(const std::string &zip_path, const std::string & src_dir
    , const std::string &prefix_dir
    , const std::string &current_dir ) {
    std::string path = src_dir + current_dir;
    QDir dir(path.c_str());
    if (!dir.exists()) {
        qWarning() << "add_zip : dir not exist : " << path.c_str();
        return false;
    }
    QStringList const files = dir.entryList(QDir::Files);
    if (files.isEmpty()) {
        qDebug() << " No files in dir : " << src_dir.c_str();
        return false;
    }
    for (QString const& f : files) {
        ZipFile::AddFile(zip_path
            , path + f.toStdString()
            , prefix_dir + current_dir + f.toStdString()
//            , LzmaMethod::Create()
        );
    }

    QStringList const folders = dir.entryList(QDir::NoDot | QDir::NoDotDot | QDir::Dirs);
    for (QString const& f : folders) {
        if (!add_zip(zip_path, src_dir, prefix_dir, current_dir + f.toStdString() + "/"))
            return false;
    }
    return true;
}


bool cHTTPRequest::upload_routes(const QString& ip_address, const QList<QString> &route_dirs) {
    // zip all files in route_dir
    std::string zip_path = tmp_dir.absoluteFilePath("route.zip").toStdString();
    upload_file.setFileName(zip_path.c_str());
    upload_file.remove();

    foreach(const QString & route_dir, route_dirs){
        QDir dir(route_dir);
        QString route_dir2;
        if (route_dir.length() > 0
            && route_dir[route_dir.length() - 1] != '/'
            && route_dir[route_dir.length() - 1] != '\\') {
            route_dir2 = route_dir + '/';
        }
        else {
            route_dir2 = route_dir;
        }
        if (!add_zip(zip_path, route_dir2.toStdString(), (dir.dirName() + '/').toStdString(), ""))
            return false;
    }

    // upload
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"upload.zip\""));

    upload_file.open(QIODevice::ReadOnly);
    part.setBodyDevice(&upload_file);
    upload_file.setParent(multiPart);

    multiPart->append(part);

    QString url_str = QString("http://") + ip_address + ":"
        + QString::number(PORT) + "/api/upload_routes";
    QUrl url(url_str);
    QNetworkRequest request(url);
    QNetworkAccessManager* manager = new QNetworkAccessManager;

    //QNetworkReply *reply = manager->post(request, multiPart);
    reply = manager->post(request, multiPart);
    multiPart->setParent(reply);
    qDebug() << "upload_route";

    connect(reply, &QNetworkReply::finished, this
            , [=]() {
        qDebug() << " fin";
        if (reply->error()) {
            qWarning() << reply->errorString();
            emit signal_progress_upload_routes( true );
        }
        else{
            std::string s = reply->readAll().toStdString();
            qDebug() << "reply : " << s.c_str();
            QJsonDocument json = QJsonDocument::fromJson(s.c_str());
            if (json.isNull()) {
                qWarning() << "Invalid response from vmer : " << s.c_str();
                emit signal_progress_upload_routes( true );
            }
            else if (json["result"].toString() == "success") {
                QList<QString> route_names;
                foreach(const QJsonValue & r, json["routes"].toArray()){
                    qDebug() << " Route : " << r.toString();
                    route_names.append(r.toString());
                }
                emit signal_progress_upload_routes( false, 1.0, route_names );
            }
            else{
                emit signal_progress_upload_routes( true );
            }
            qDebug() << " upload fin";
            upload_file.close();
            upload_file.remove();
        }
        if( this->reply ){
//            this->reply->deleteLater();
            this->reply = NULL;
        }
    });
    connect(reply, &QNetworkReply::uploadProgress, this, [=](qint64 ist, qint64 max) {
        qDebug() << " upload progress : " << ist << " / " << max;
        if( ist > 0 && ist!=max )
            emit signal_progress_upload_routes( false, float(ist)/max, QList<QString>() );
    });
    connect(reply, &QNetworkReply::errorOccurred, this, [=](QNetworkReply::NetworkError e) {
        qWarning() << "Network error : " << e;
        upload_file.close();
        upload_file.remove();
        emit signal_progress_upload_routes( true );
        if( this->reply ){
//            this->reply->deleteLater();
            this->reply = NULL;
        }
    });

    return true;
}



bool cHTTPRequest::get_route_list(const QString& ip_address) {
    auto manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished, this
        , [=](QNetworkReply* reply) {
            if (reply->error()) {
                qWarning() << reply->errorString();
                emit signal_get_route_list( true );
                return;
            }
            std::string s = reply->readAll().toStdString();
            qDebug() << "reply : " << s.c_str();
            QJsonDocument json = QJsonDocument::fromJson(s.c_str());
            if (json.isNull()) {
                qWarning() << "Invalid response from vmer : " << s.c_str();
                emit signal_get_route_list( true );
            }
            else if (json["result"].toString() == "success") {
                QList<QString> route_names;
                foreach(const QJsonValue & r, json["routes"].toArray()){
                    qDebug() << " Route : " << r.toString();
                    route_names.append(r.toString());
                }
                emit signal_get_route_list( false, route_names );
            }
            else {
                qWarning() << "Error msg : " << json["msg"];
                emit signal_get_route_list( true );
            }
        });

    QString url = QString("http://") + ip_address + ":"
        + QString::number(PORT) + "/api/get_route_list";
    qDebug() << "url : " << url;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    manager->get(request);
    return true;
}


bool cHTTPRequest::remove_routes(const QString& ip_address, const QList<QString> &route_names) {
    auto manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished, this
        , [=](QNetworkReply* reply) {
            if (reply->error()) {
                qWarning() << reply->errorString();
                emit signal_remove_routes( true );
                return;
            }
            std::string s = reply->readAll().toStdString();
            qDebug() << "reply : " << s.c_str();
            QJsonDocument json = QJsonDocument::fromJson(s.c_str());
            if (json.isNull()) {
                qWarning() << "Invalid response from vmer : " << s.c_str();
                emit signal_remove_routes( true );
            }
            else if (json["result"].toString() == "success") {
                QList<QString> new_route_names;
                foreach(const QJsonValue & r, json["routes"].toArray()){
                    qDebug() << " Route : " << r.toString();
                    new_route_names.append(r.toString());
                }
                emit signal_remove_routes( false, new_route_names );
            }
            else{
                emit signal_remove_routes( true );
            }
        });

    QString str;
    foreach( const QString &route_name, route_names){
        if( str.length()>0 )
            str+= ",";
        str+= "\""+route_name+"\"";
    }
    QUrl url(QString("http://") + ip_address + ":"
        + QString::number(PORT) + "/api/remove_routes?data={\"route_names\":[" + str +"]}");
    /*
    QUrl url(QString("http://") + ip_address + ":"
        + QString::number(PORT) + "/api/remove_routes");
    QUrlQuery query;
    query.addQueryItem("route_name", route_name);
    url.setQuery(query.query());
    */
    qDebug() << "url : " << url.toString();

    QNetworkRequest request;
    request.setUrl(url);
    manager->get(request);
    return true;
}


void cHTTPRequest::discover_devices() {
    QByteArray datagram = "discover_devices";
    devices.clear();
    qDebug() << "discover_devices()";
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, PORT_UDP);
}


void cHTTPRequest::devices_found() {
    QByteArray buffer;
    QHostAddress sender;
    buffer.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(buffer.data(), buffer.size(), &sender);

    auto str = buffer.toStdString();
    qDebug() << "reply : " << str.c_str();
    QJsonDocument json = QJsonDocument::fromJson(str.c_str());
    if (json.isNull()) {
        qWarning() << "Invalid response from vmer : " << str.c_str();
        emit signal_device_found( true );
    }
    else if (json["result"].toString() == "success") {
        tDeviceInfo device;
        QStringList l = sender.toString().split(":");
        device.ip_address = l[l.size()-1];
        device.name = json["device_name"].toString();
        device.id = json["device_id"].toString();
        qDebug() << "device : " << device.ip_address << 
            " : id =" << device.id << ", name = " << device.name;
        for(int i=0;i<devices.length();i++){
            if( device.ip_address==devices[i].ip_address ){
                qWarning() << "devices found 2 times : name = " << device.name;
                return;
            }
        }
        devices.push_back(device);
        emit signal_device_found( false, device );
    }
    else {
        qWarning() << "Invalid device info : " << str.c_str() 
            << " , from : " << sender;
        emit signal_device_found( true );
    }
}


bool cHTTPRequest::extract(const QString& file_path, const QString& dst) {
    ZipArchive::Ptr archive = ZipFile::Open(file_path.toStdString());
    size_t entries = archive->GetEntriesCount();

    QDir dir;
    if (!dir.exists(dst)) {
        if(!dir.mkdir(dst)){
            qWarning() << "extract : Cannot create dir : " + dst;
            return false;
        }
    }
    for (size_t i = 0; i < entries; ++i)
    {
        auto entry = archive->GetEntry(int(i));
        std::istream* decompressStream = entry->GetDecompressionStream();
        QString p = (dst + entry->GetFullName().c_str());
        qDebug() << " unzip results : " << p;
        std::ofstream ofs(p.toStdString()
            , std::ios::binary);
        char buffer[1024];
        std::streamsize n;
        decompressStream->read(buffer, sizeof(buffer));
        while ((n = decompressStream->gcount()) != 0) {
            ofs.write(buffer, n);
            decompressStream->read(buffer, sizeof(buffer));
        }
        ofs.close();
    }
    qDebug() << " Files download : " << entries;
    return true;
}


bool cHTTPRequest::cancel_task(){
    qDebug() << "cancel_task()";
    if( reply && reply->isRunning() ){
        reply->abort();
        return true;
    }
    else{
        qWarning() << "cancel_task() : no task running";
        return false;
    }
}

QString create_random_string(const int len)
{
    const QString possibleCharacters("abcdefghijklmnopqrstuvwxyz0123456789");

    QString s;
    for (int i = 0; i < len; ++i)
    {
        int index = QRandomGenerator::global()->generate() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        s.append(nextChar);
    }
    return s;
}


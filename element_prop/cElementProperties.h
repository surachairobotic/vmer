#ifndef CELEMENTPROPERTIES_H
#define CELEMENTPROPERTIES_H

#include "mainwindow.h"
#include <QtCore>
#include <QtWidgets/QItemDelegate>
#include <QtWidgets/QLineEdit>


class cElementProperties : public QObject{
Q_OBJECT
public:
    cElementProperties(MainWindow *_mainwindow);
    ~cElementProperties();

    bool init();

    // load JSON from string and show properties on tableWidgetElementProperties
    bool load_json(const QString& json_str);

    // load JSON from text file
    bool load_json_file(const QString& path);

    // create JSON string from properties in tableWidgetElementProperties
    bool create_json(QString &json_str);

private:
    MainWindow *mainwindow;
    QJsonDocument json;
};


#endif // CELEMENTPROPERTIES_H

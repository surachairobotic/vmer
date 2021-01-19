#pragma once
#include <QtCore>
#include <QList>

class cPoint {
public:
	QString name, config;
	cPoint(const QString _name, const QString _config) :name(_name), config(_config) {}
};

class cElement {
public:
	QString name;
	QList<cPoint> points;
	cElement(const QString _name) :name(_name) {}
};

class cMachine {
public:
	QString name;
	QList<cElement> elements;
	cMachine(const QString _name) :name(_name) {}
};

class cRoute {
public:
	void print() {
		qDebug() << "Route : " << name;
		for (int i = 0; i < machines.size(); i++) {
			qDebug() << "  Machine : " << machines[i].name;
			for (int j = 0; j < machines[i].elements.size(); j++) {
				qDebug() << "    Element : " << machines[i].elements[j].name;
				for (int k = 0; k < machines[i].elements[j].points.size(); k++) {
					qDebug() << "      Point : " << machines[i].elements[j].points[k].name
						<< "\n        " << machines[i].elements[j].points[k].config;
				}
			}
		}
	}

	QString name;
	QList<cMachine> machines;
};

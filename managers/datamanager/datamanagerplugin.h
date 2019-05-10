#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef DATAMANAGERPLUGIN_H
#define DATAMANAGERPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include "interfaces/databaseinterface.h"
#include "interfaces/viewerinterface.h"

class DataManagerPlugin : public QObject, DatabaseInterface, ViewerInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hwsdsw.HydroCurve.DatabaseInterface" FILE "datamanager.json")
    Q_INTERFACES(DatabaseInterface)
    Q_INTERFACES(ViewerInterface)

    // DatabaseInterface interface
public:
    void setDatabase(QSqlDatabase db);

    // ViewerInterface interface
public:
    QWidget *create(QWidget *parent);
    QString name();
};

#endif

#ifndef DATABASEDATAPROVIDERPLUGIN_H
#define DATABASEDATAPROVIDERPLUGIN_H

#include <QtCore>
#include <QtPlugin>
#include <interfaces/databaseinterface.h>
#include <interfaces/dataproviderinterface.h>

class DatabaseDataProviderPlugin : public QObject, DatabaseInterface, DataProviderInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hwsdsw.HydroCurve.DatabaseInterface" FILE "databasedataprovider.json")
    Q_INTERFACES(DatabaseInterface)
    Q_INTERFACES(DataProviderInterface)

    // DatabaseInterface interface
public:
    void setDatabase(QSqlDatabase db);

    // DataProviderInterface interface
public:
    QString type();

    QList<int> zqs_process_year_list();
    QStringList zq_process_stcd_list();
};

#endif // DATABASEDATAPROVIDERPLUGIN_H

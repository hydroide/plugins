#ifndef SQLITEDATAPROVIDERPLUGIN_H
#define SQLITEDATAPROVIDERPLUGIN_H

#include <QtCore>
#include <QtPlugin>
#include <interfaces/databaseinterface.h>
#include <interfaces/dataproviderinterface.h>

class SqliteDataProviderPlugin : public QObject, DatabaseInterface, DataProviderInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hwsdsw.HydroCurve.DatabaseInterface" FILE "sqlitedataprovider.json")
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

    QMap<QDateTime, QString> z_series(const QString &stcd, const QDateTime &startDateTime, const QDateTime &endDateTime);
    QMap<QDateTime, QString> rq_series(const QString &stcd, const QDateTime &startDateTime, const QDateTime &endDateTime);
    QMap<QDateTime, QString> s_series(const QString &stcd, const QDateTime &startDateTime, const QDateTime &endDateTime);

    QMap<QDateTime, DGMeasuredDischarge> mq_series(const QString &stcd, const QDateTime &startDateTime, const QDateTime &endDateTime);
    QMap<QDateTime, DGMeasuredSedimentDischarge> msq_series(const QString &stcd, const QDateTime &startDateTime, const QDateTime &endDateTime);

    RelationCurve zq_curve(const QString &stcd, int year, int curve_id);
    QList<DGSDRPeriod> zq_curves_periods(const QString &stcd, int year);

private:
    QSqlDatabase _db;

};

#endif // SQLITEDATAPROVIDERPLUGIN_H

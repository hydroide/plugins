#include "databasedataproviderplugin.h"

QSqlDatabase DB;

void DatabaseDataProviderPlugin::setDatabase(QSqlDatabase db)
{
    DB = db;
}

QString DatabaseDataProviderPlugin::type()
{
    return QString("database");
}

QList<int> DatabaseDataProviderPlugin::zqs_process_year_list()
{
    QList<int> list;

    auto db = DB;
    if (db.isValid() && db.open())
    {
        QSqlQuery q(db);

        QString sql = "SELECT DISTINCT(year) "
                      "FROM ZQ_PROCESS "
                      "UNION "
                      "SELECT DISTINCT(year) "
                      "FROM SQ_PROCESS "
                      "ORDER BY year ASC;";

        if (q.exec(sql))
        {
            while (q.next())
            {
                list.append(q.value("year").toInt());
            }
        }
        else
        {
            qDebug() << q.lastQuery() << q.lastError();
        }

        db.close();
    }
    return list;
}

QStringList DatabaseDataProviderPlugin::zq_process_stcd_list()
{
    QStringList list;

    auto db = DB;
    if (db.isValid() && db.open())
    {
        QSqlQuery q(db);

        QString sql = "SELECT DISTINCT(stcd) "
                      "FROM ZQ_PROCESS "
                      "ORDER BY stcd ASC;";

        if (q.exec(sql))
        {
            while (q.next())
            {
                list.append(q.value("stcd").toString());
            }
        }
        else
        {
            qDebug() << q.lastQuery() << q.lastError();
        }

        db.close();
    }

    return list;
}

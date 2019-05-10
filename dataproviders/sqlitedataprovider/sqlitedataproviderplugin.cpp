#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "sqlitedataproviderplugin.h"

#include <boost/multiprecision/cpp_dec_float.hpp>
using Dec = boost::multiprecision::cpp_dec_float_50;

#include "helpers/datetimehelper.h"
#include "helpers/mathhelper.h"
#include "helpers/roundinghelper.h"
#include "models/point.h"

#include "database/sqls.h"

void SqliteDataProviderPlugin::setDatabase(QSqlDatabase db)
{
    _db = db;
}

bool SqliteDataProviderPlugin::initDatabase()
{
    auto db = _db;

    if (!(db.isValid() && db.open()))
    {
        return false;
    }

    bool ret = true;

    QSqlQuery q(db);
    ret &= q.exec(DROPEcss);
    ret &= q.exec(DROPEcssv);
    ret &= q.exec(DROPEsccn);
    ret &= q.exec(DROPEscp);
    ret &= q.exec(DROPEscr);
    ret &= q.exec(DROPEsqm);
    ret &= q.exec(DROPEstn);
    ret &= q.exec(DROPEzqcn);
    ret &= q.exec(DROPEzqm);
    ret &= q.exec(DROPEzqp);
    ret &= q.exec(DROPEzqr);

    ret &= q.exec(CRTNEcss);
    ret &= q.exec(CRTNEcssv);
    ret &= q.exec(CRTNEsccn);
    ret &= q.exec(CRTNEscp);
    ret &= q.exec(CRTNEscr);
    ret &= q.exec(CRTNEsqm);
    ret &= q.exec(CRTNEstn);
    ret &= q.exec(CRTNEzqcn);
    ret &= q.exec(CRTNEzqm);
    ret &= q.exec(CRTNEzqp);
    ret &= q.exec(CRTNEzqr);
    return ret;
}

QString SqliteDataProviderPlugin::type()
{
    return QString("sqlite");
}

QList<int> SqliteDataProviderPlugin::zqs_process_year_list()
{
    QList<int> list;

    auto db = _db;
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

QStringList SqliteDataProviderPlugin::zq_process_stcd_list()
{
    QStringList list;

    auto db = _db;
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

QMap<QDateTime, QString> SqliteDataProviderPlugin::z_series(
        const QString &stcd,
        const QDateTime &startDateTime,
        const QDateTime &endDateTime)
{
    QMap<QDateTime, QString> map;
    auto db = _db;
    if (db.isValid() && db.open())
    {
        QSqlQuery q(db);

        if (startDateTime > endDateTime)
        {
            return map;
        }

        QString sqlclause = "FROM ZQ_PROCESS "
                            "WHERE stcd=:stcd "
                            "AND DATETIME(timestamp) >= DATETIME(:start) "
                            "AND DATETIME(timestamp) <= DATETIME(:end) "
                            "AND DATETIME(timestamp) <= DATETIME(:end) "
//                            "AND Strftime('%M%S', timestamp) = '0000' "
                            "ORDER BY DATETIME(timestamp) ASC;";

        q.clear();
        q.prepare("SELECT timestamp, stage " + sqlclause);

        q.bindValue(":stcd", stcd);
        q.bindValue(":start", startDateTime);
        q.bindValue(":end", endDateTime);

        if (q.exec())
        {
            while (q.next())
            {
                map.insert(q.value("timestamp").toDateTime(),
                           q.value("stage").toString());
            }
        }
        else
        {
            qDebug() << q.lastQuery() << q.lastError();
        }

        db.close();
    }
    return map;
}

QMap<QDateTime, QString> SqliteDataProviderPlugin::q_series(
        const QString &stcd,
        const QDateTime &startDateTime,
        const QDateTime &endDateTime)
{
    auto stageMap = z_series(stcd, startDateTime, endDateTime);

    QMap<QDateTime, QString> dischargeMap;

    QMap<int, QMap<int, QVector<Point<Dec>>>> yearCurveNodes; // 懒加载

    QMap<int, QList<DGSDRPeriod>> periodMap;

    for (auto key : stageMap.keys())
    {
        int year = key.date().year();
        QList<qint64> timeSpans;
        if (!periodMap.contains(year))
        {
            periodMap[year] = zq_curves_periods(stcd, year);
        }
        auto periods = periodMap[year];

        for (const auto & period: periods)
        {
            timeSpans.append(period.end_timestamp.toMSecsSinceEpoch()
                             - key.toMSecsSinceEpoch());
        }
        if (timeSpans.size() > 0)
        {
            int id{0}, minId{0};
            for (auto ts: timeSpans)
            {
                if (ts >= 0)
                {
                    minId = id;
                    break;
                }
                id++;
            }
            auto period = periods.at(minId);
            if (!yearCurveNodes.contains(year))
            {
                yearCurveNodes.insert(year, QMap<int, QVector<Point<Dec>>>());
            }
            if (!yearCurveNodes[year].contains(period.curve_id))
            {
                auto curve = zq_curve(stcd, year, period.curve_id);
                auto nodes = curve.nodes;

                QVector<Point<Dec>> points;
                for (auto node : nodes)
                {
                    Dec stage(node.y.toStdString());
                    Dec discharge(node.x.toStdString());
                    Point<Dec> point(stage, discharge);
                    points.append(point);
                }
                yearCurveNodes[year].insert(period.curve_id, points);
            }

            auto stage = stageMap.value(key);
            bool ok = false;
            stage.toFloat(&ok);
            if (ok)
            {
                auto discharge = MathHelper::PLPI_MPS<Dec>(
                            yearCurveNodes.value(year).value(period.curve_id),
                            Dec(stage.toStdString()));
                dischargeMap.insert(key, QString::fromStdString(
                                        RoundingHelper::roundDischarge(discharge)));
            }
        }
    }

    return dischargeMap;
}

QMap<QDateTime, QString> SqliteDataProviderPlugin::s_series(
        const QString &stcd,
        const QDateTime &startDateTime,
        const QDateTime &endDateTime)
{
    QMap<QDateTime, QString> map;

    auto db = _db;
    if (db.isValid() && db.open())
    {

        QSqlQuery q(db);

        auto runAndInsert = [&]() {
            if (q.exec())
            {
                while (q.next())
                {
                    map.insert(q.value("timestamp").toDateTime(),
                               q.value("concentration").toString());
                }
            }
        };

        QString sql;
        sql = "SELECT timestamp, concentration "
              "FROM SQ_PROCESS "
              "WHERE stcd=:stcd "
              "AND DATETIME(timestamp) >= DATETIME(:start) "
              "AND DATETIME(timestamp) <= DATETIME(:end) "
              "ORDER BY DATETIME(timestamp) ASC;";
        q.prepare(sql);
        q.bindValue(":stcd", stcd);
        q.bindValue(":start", startDateTime);
        q.bindValue(":end", endDateTime);
        runAndInsert();

        sql = "SELECT timestamp, concentration "
              "FROM SQ_PROCESS "
              "WHERE stcd=:stcd "
              "AND DATETIME(timestamp) < DATETIME(:start) "
              "ORDER BY DATETIME(timestamp) DESC "
              "LIMIT 1;";
        q.prepare(sql);
        q.bindValue(":stcd", stcd);
        q.bindValue(":start", startDateTime);
        runAndInsert();

        sql = "SELECT timestamp, concentration "
              "FROM SQ_PROCESS "
              "WHERE stcd=:stcd "
              "AND DATETIME(timestamp) > DATETIME(:end) "
              "ORDER BY DATETIME(timestamp) ASC "
              "LIMIT 1;";
        q.prepare(sql);
        q.bindValue(":stcd", stcd);
        q.bindValue(":end", endDateTime);
        runAndInsert();

        db.close();
    }
    return map;
}

QMap<QDateTime, DGMeasuredDischarge> SqliteDataProviderPlugin::mq_series(
            const QString &stcd,
            const QDateTime &startDateTime,
            const QDateTime &endDateTime)
{
    QMap<QDateTime, DGMeasuredDischarge> map;
    auto db = _db;
    if (db.isValid() && db.open())
    {
        QSqlQuery q(db);

        QString sqlMeasuredDischarge = "SELECT timestamp, mmsn, stage, discharge "
                                       "FROM ZQ_MEASUREMENT "
                                       "WHERE stcd = :stcd "
                                       "AND DATETIME(timestamp) >= DATETIME(:start) "
                                       "AND DATETIME(timestamp) <= DATETIME(:end) "
                                       "ORDER BY DATETIME(timestamp);";
        q.prepare(sqlMeasuredDischarge);
        q.bindValue(":stcd", stcd);
        q.bindValue(":start", startDateTime);
        q.bindValue(":end", endDateTime);

        if (q.exec())
        {
            while (q.next())
            {
                DGMeasuredDischarge obj;
                obj.stcd = stcd;
                obj.averageDateTime = q.value("timestamp").toDateTime();
                obj.sn = q.value("mmsn").toString();
                obj.stage = q.value("stage").toString();
                obj.discharge = q.value("discharge").toString();
                map.insert(obj.averageDateTime, obj);
            }
        }

        db.close();
    }
    return map;
}

QMap<QDateTime, DGMeasuredSedimentDischarge> SqliteDataProviderPlugin::msq_series(
        const QString &stcd,
        const QDateTime &startDateTime,
        const QDateTime &endDateTime)
{
    QMap<QDateTime, DGMeasuredSedimentDischarge> map;
    auto db = _db;
    if (db.isValid() && db.open())
    {

        QSqlQuery q(db);

        QString sql = "SELECT timestamp, mmsn, discharge, cssqr "
                      " , csmsc "
                      " , sssc "
                      "FROM SQ_MEASUREMENT "
                      "WHERE stcd = :stcd "
                      "AND DATETIME(timestamp) >= DATETIME(:start) "
                      "AND DATETIME(timestamp) <= DATETIME(:end) "
                      "ORDER BY DATETIME(timestamp);";
        q.prepare(sql);
        q.bindValue(":stcd", stcd);
        q.bindValue(":start", startDateTime);
        q.bindValue(":end", endDateTime);

        if (q.exec())
        {
            while (q.next())
            {
                DGMeasuredSedimentDischarge obj;
                obj.sn = q.value("mmsn").toString();
                obj.discharge = q.value("discharge").toString();
                obj.sedimentDischarge = q.value("cssqr").toString();
                obj.crossSectionalMeanSedimentConcentration = q.value("csmsc").toString();
                obj.singleSampleSedimentConcentration = q.value("sssc").toString();
                map.insert(q.value("timestamp").toDateTime(), obj);
            }
        }

        db.close();
    }
    return map;
}

RelationCurve SqliteDataProviderPlugin::zq_curve(const QString &stcd, int year, int curve_id)
{
    RelationCurve obj;
    obj.stcd = stcd;
    obj.year = year;
    obj.curve_id = curve_id;
    auto db = _db;
    if (db.isValid() && db.open())
    {
        QSqlQuery q(db);

        QString sqlNodes = "SELECT stage, discharge "
                           "FROM ZQ_CURVE_NODE "
                           "WHERE stcd=:stcd "
                           "AND year=:year "
                           "AND curve_id=:curve_id "
                           "ORDER BY node_id; ";

        q.clear();
        q.prepare(sqlNodes);
        q.bindValue(":stcd", stcd);
        q.bindValue(":year", year);
        q.bindValue(":curve_id", curve_id);
        if (q.exec())
        {
            while (q.next())
            {
                RelationCurveNode node;
                node.y = q.value("stage").toString();
                node.x = q.value("discharge").toString();
                obj.nodes.append(node);
            }
        }
        else
        {
            qDebug() << q.lastQuery() << q.lastError();
        }
        db.close();
    }
    return obj;
}

QList<DGSDRPeriod> SqliteDataProviderPlugin::zq_curves_periods(const QString &stcd, int year)
{
    QList<DGSDRPeriod> list;

    auto db = _db;
    if (db.isValid() && db.open())
    {
        QSqlQuery q(db);

        QString sql = "SELECT end_time, method_id, curve_id, period_id "
                      "FROM ZQ_PERIOD "
                      "WHERE stcd=:stcd "
                      "AND year=:year "
                      "ORDER BY period_id ASC; ";

        q.prepare(sql);
        q.bindValue(":stcd", stcd);
        q.bindValue(":year", year);
        if (q.exec())
        {
            while (q.next())
            {
                DGSDRPeriod period;
                period.period_id = q.value("period_id").toInt();
                period.method_id = q.value("method_id").toInt();
                period.curve_id = q.value("curve_id").toInt();
                //            period.end_time = q.value("end_time").toString();
                period.end_timestamp = DateTimeHelper::fromShortFormat(
                            q.value("end_time").toString(), year);
                list.append(period);
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

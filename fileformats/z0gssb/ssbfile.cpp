#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "ssbfile.h"

#include <QtDebug>
#include <QtWidgets>

#ifdef QT_SQL
#include <QtSql>
#endif

//#include "models/dmeasurementrecord.h"
//#include "models/sdmeasurementrecord.h"
#include "models/table.h"

#include "helpers/datetimehelper.h"

#include "z0gssbplugin.h"

SsbFile::SsbFile(const QString& filename)
{
    _pfile = new QFile(filename);
}

void SsbFile::readFile()
{
    if (!_pfile->exists())
    {
        return;
    }

    //    Q_ASSERT(spData != nullptr);

    if (!_pfile->open(QFile::ReadOnly))
    {
        throw std::runtime_error("SSB file coule not be read");
    }
#ifdef QT_SQL
    {
        auto db = Z0gSsbPlugin::DB;
        if (!db.isValid()) {
            QMessageBox::critical(nullptr, QObject::tr("错误"), QObject::tr("数据库不正常"));
            return;
        }
        if (!db.open()) {
            QMessageBox::critical(nullptr, QObject::tr("错误"), QObject::tr("数据库打开失败"));
            return;
        }
        db.transaction();
        QSqlQuery q(db);

        QTextStream in(_pfile);
        in.setCodec("gbk");
        QString line1 = in.readLine(); // 域宽
        int n = line1.length();

        QString line2 = in.readLine(); // 站码 年份与记录数
        QString stcd = line2.mid(0*n, n).trimmed();
        int year = line2.mid(1*n, n).trimmed().toInt();
        int totalRecordCount = line2.mid(2*n, n).trimmed().toInt();

        q.exec("BEGIN;");

        QProgressDialog progress(QObject::tr("正在导入：") + _pfile->fileName(),
                                 QObject::tr("取消"), 0, totalRecordCount, nullptr);
        progress.setWindowModality(Qt::WindowModal);
        int i = 0;

        while(!in.atEnd())
        {
            if (progress.wasCanceled())
            {
                return;
            }
            progress.setValue(i++);

            QString line = in.readLine();
            line = line.trimmed();
            QStringList list = line.split(',');
            if (list[0] == "ZHZBSCLLCGB") // 实测流量
            {
                static int lastYear{0};
                static int lastMonth{0};
                static int lastDay{0};
                int year = list[2].toInt();
                if (0 == year)
                {
                    list[2] = QString::number(lastYear);
                }
                else
                {
                    lastYear = year;
                }
                int month = list[5].toInt();
                if (0 == month)
                {
                    list[5] = QString::number(lastMonth);
                }
                else
                {
                    lastMonth = month;
                }
                int day = list[6].toInt();
                if (0 == day)
                {
                    list[6] = QString::number(lastDay);
                }
                else
                {
                    lastDay = day;
                }

                static QString lastLocation = "";
                if (SYMBOLS_AS_RECENT.contains(list[9]))
                {
                    list[9] = lastLocation;
                }
                else
                {
                    lastLocation = list[9];
                }
                QString stage = list[10];
                static QString lastMeter = "";
                if (stage.contains("."))
                {
                    lastMeter = stage.split(".")[0];
                }
                else
                {
                    list[10] = lastMeter + "." + stage;
                }

                static QString lastMethod = "";
                if (SYMBOLS_AS_RECENT.contains(list[21]))
                {
                    list[21] = lastMethod;
                }
                else
                {
                    lastMethod = list[21];
                }

                QTime start_time(DateTimeHelper::fromMMSS(list[7]));
                QTime end_time(DateTimeHelper::fromMMSS(list[8]));
                QDate date(list[2].toInt(), list[5].toInt(), list[6].toInt());
                QDateTime startDT(date, start_time);
                QDateTime endDT(date, end_time);
                if (end_time < start_time) // 跨日
                {
                    endDT.addDays(1);
                }
                //            qDebug() << start_time << end_time << date;
                QDateTime averageDT = DateTimeHelper::averageDT(startDT, endDT);
                //            qDebug() << averageDT << averageDT.toString(DEFAULT_DATETIME_FORMAT);
                list.append(averageDT.toString(DEFAULT_DATETIME_FORMAT));

                list.removeAt(0);

                std::for_each(list.begin(), list.end(), [](QString &str){
                    str = QString("'%1'").arg(str);
                });
                QString sql = QString("INSERT INTO ZQ_MEASUREMENT "
                                      "VALUES(NULL, %1)").arg(list.join(","));
                if (q.exec(sql))
                {

                }
                else
                {
                    qDebug() << q.lastQuery() << q.lastError();
                }
                //            spData->_vDMeasurementRecords.push_back(record);

            }
            else if (list[0] == "ZHZBSCXYZSSL") // 实测输沙率
            {
                static int lastYear{0};
                static int lastMonth{0};
                static int lastDay{0};

                int year = list[2].toInt();
                if (0 == year)
                {
                    list[2] = QString::number(lastYear);
                }
                else
                {
                    lastYear = year;
                }

                int month = list[6].toInt();
                if (0 == month)
                {
                    list[6] = QString::number(lastMonth);
                }
                else
                {
                    lastMonth = month;
                }

                int day = list[7].toInt();
                if (0 == day)
                {
                    list[7] = QString::number(lastDay);
                }
                else
                {
                    lastDay = day;
                }

                static QString lastIsMethod = "";
                if (SYMBOLS_AS_RECENT.contains(list[14]))
                {
                    list[14] = lastIsMethod;
                }
                else
                {
                    lastIsMethod = list[14];
                }

                static QString lastEquipmentType = "";
                if (SYMBOLS_AS_RECENT.contains(list[16]))
                {
                    list[16] = lastEquipmentType;
                }
                else
                {
                    lastEquipmentType = list[16];
                }

                static QString lastMethod = "";
                if (SYMBOLS_AS_RECENT.contains(list[18]))
                {
                    list[18] = lastMethod;
                }
                else
                {
                    lastMethod = list[18];
                }


                QTime start_time(DateTimeHelper::fromMdotSS(list[8]));
                QTime end_time(DateTimeHelper::fromMdotSS(list[9]));
                QDate date(list[2].toInt(), list[6].toInt(), list[7].toInt());
                QDateTime startDT(date, start_time);
                QDateTime endDT(date, end_time);
                if (end_time < start_time) // 跨日
                {
                    endDT.addDays(1);
                }
                QDateTime averageDT = DateTimeHelper::averageDT(startDT, endDT);
                list.append(averageDT.toString(DEFAULT_DATETIME_FORMAT));

                list.removeAt(0);

                std::for_each(list.begin(), list.end(), [](QString &str){
                    str = QString("'%1'").arg(str);
                });
                QString sql = QString("INSERT INTO "
                                      "SQ_MEASUREMENT "
                                      "VALUES(NULL, %1)").arg(list.join(","));
                if (q.exec(sql))
                {

                }
                else
                {
                    qDebug() << q.lastQuery() << q.lastError();
                }
                //            spData->_vSDMeasurementRecords.push_back(record);
            }
            else if (list[0] == "ZHZBDDMCGBH") // 实测大断面标记
            {
                //            CSSurveyRecord record;
                //            record._stcd = list[1];
                //            record._year = list[2].toInt();
                //            record._survey_id = list[3].toInt();
                //            record._monthDay = list[4];
                //            record._location = list[5];
                //            record._stage = list[6];
                //            record._annotation = list[7];
                //            record._annoation = list[8];

                //            spData->_vCS_SURVEY.push_back(record);

                list.removeAt(0);
                list.append(QDateTime::currentDateTime().toString(DEFAULT_DATETIME_FORMAT));
                std::for_each(list.begin(), list.end(), [](QString &str){
                    str = QString("'%1'").arg(str);
                });
                QString sql = QString("INSERT INTO CS_SURVEY "
                                      "VALUES(NULL, %1)").arg(list.join(","));
                if (q.exec(sql))
                {

                }
                else
                {
                    qDebug() << q.lastQuery() << q.lastError();
                }
            }
            else if (list[0] == "ZHZBDDMCGB") // 实测大断面内容
            {
                list.removeAt(8);
                list.removeAt(0);
                list.append(QDateTime::currentDateTime().toString(DEFAULT_DATETIME_FORMAT));
                std::for_each(list.begin(), list.end(), [](QString &str){
                    str = QString("'%1'").arg(str);
                });
                QString sql = QString("INSERT INTO CS_VERTICAL "
                                      "VALUES(NULL, %1)").arg(list.join(","));
                if (q.exec(sql))
                {

                }
                else
                {

                    qDebug() << q.lastQuery() << q.lastError();
                }
            }
        }
        q.exec("END;");

        progress.setValue(totalRecordCount);

        if (!db.commit()){
            db.rollback();
        }
        db.close();
    }
#endif
}

void SsbFile::writeFile()
{
    if (_pfile == nullptr)
    {
        return;
    }
}

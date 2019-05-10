#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "z0gfile.h"

#include <QtDebug>
#include <QtWidgets>

#ifdef QT_SQL
#include <QtSql>
#endif

#include "hcglobaldefs.h"

#include "models/table.h"
#include "models/station.h"
#include "helpers/datetimehelper.h"

#include "z0gssbplugin.h"

Z0gFile::Z0gFile(const QString &filepath)
    : ZBFile(filepath)
{

}

void Z0gFile::readFile()
{
    if (!_pfile->exists())
    {
        return;
    }

    //    Q_ASSERT(sp != nullptr);

    if (!_pfile->open(QFile::ReadOnly))
    {
        throw std::runtime_error("Z0G file coule not be read");
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

        QString line1 = in.readLine();
        int n = line1.length(); // 第一行定义数据域宽

        QString line2 = in.readLine(); // 第二行定义站码年份
        QString stcd = line2.mid(0*n, n).trimmed();
        int year = line2.mid(1*n, n).trimmed().toInt();
        int totalRecordCount = line2.mid(2*n, n).trimmed().toInt();

        QList<Table> tables;
        tables << Table("ZQ_PERIOD")
               << Table("SQ_PERIOD")
               << Table("ZQ_CURVE_NODE")
               << Table("SQ_CURVE_NODE")
               << Table("SQ_PROCESS")
               << Table("ZQ_PROCESS");
        if (!checkDuplication(db, stcd, year, tables)) {
            db.close();
            return;
        }

        QString line;

        q.exec("BEGIN;");


        QProgressDialog progress(QObject::tr("正在导入：") + _pfile->fileName(),
                                 QObject::tr("取消"), 0, totalRecordCount, nullptr);
        progress.setWindowModality(Qt::WindowModal);
        int i = 0;

        while (!in.atEnd())
        {
            if (progress.wasCanceled())
            {
                return;
            }
            progress.setValue(i++);

            line = in.readLine();

            QStringList list;

            for (auto i = 0; i < line.length()/n; ++i) {
                list << line.mid(i*n, n).trimmed();
            }


            QString tag;

            if (list.length() > 0)
            {
                tag = list[0];
            }

            if (tag == "Z0G01") // 时段控制
            {

            }

            if (tag == "Z0G02") // 洪要时段
            {

            }

            if (tag == "Z0G03") // 含沙量摘录时间
            {

            }

            if (tag == "Z0G04") // 推流时段
            {
                Q_ASSERT(list.length() > 9);

                q.prepare("INSERT INTO ZQ_PERIOD ("
                          "stcd ,"
                          "year ,"
                          "period_id ,"
                          "start_time ,"
                          "end_time ,"
                          "method_id ,"
                          "curve_id ,"
                          "annotation, "
                          "timestamp"
                          ")VALUES(?,?,?,?,?,?,?,?,?)");
                q.addBindValue(list[1]);
                q.addBindValue(list[2]);
                q.addBindValue(list[3]);
                q.addBindValue(list[4]);
                q.addBindValue(list[5]);
                q.addBindValue(list[6]);
                q.addBindValue(list[7]);
                q.addBindValue(list[8]);
                auto curr_dt_string = QDateTime::currentDateTime().toString(DEFAULT_DATETIME_FORMAT);
                q.addBindValue(curr_dt_string);
                if (q.exec())
                {

                }
                else
                {

                    qDebug() << q.lastQuery() << q.lastError();
                }
            }

            if (tag == "Z0G05") // 推沙时段
            {
                Q_ASSERT(list.length() > 8);

                q.prepare("INSERT INTO SQ_PERIOD ("
                          "stcd ,"
                          "year ,"
                          "period_id ,"
                          "start_time ,"
                          "end_time ,"
                          "method_id ,"
                          "curve_id ,"
                          "annotation, "
                          "timestamp"
                          ") VALUES(?,?,?,?,?,?,?,?,?)");
                q.addBindValue(list[1]);
                q.addBindValue(list[2]);
                q.addBindValue(list[3]);
                q.addBindValue(list[4]);
                q.addBindValue(list[5]);
                q.addBindValue(list[6]);
                q.addBindValue(list[7]);
                q.addBindValue(list[8]);
                auto curr_dt_string = QDateTime::currentDateTime().toString(DEFAULT_DATETIME_FORMAT);
                q.addBindValue(curr_dt_string);
                if (q.exec())
                {

                }
                else
                {

                    qDebug() << q.lastQuery() << q.lastError();
                }
            }

            if (tag == "Z0G06") // 推流节点
            {
                Q_ASSERT(list.length() > 6);

                q.prepare("INSERT INTO ZQ_CURVE_NODE ("
                          "stcd ,"
                          "year ,"
                          "curve_id ,"
                          "node_id ,"
                          "stage ,"
                          "discharge ,"
                          "timestamp"
                          ") VALUES(?,?,?,?,?,?,?)");
                q.addBindValue(list[1]);
                q.addBindValue(list[2]);
                q.addBindValue(list[3]);
                q.addBindValue(list[4]);
                q.addBindValue(list[5]);
                q.addBindValue(list[6]);
                auto curr_dt_string = QDateTime::currentDateTime().toString(DEFAULT_DATETIME_FORMAT);
                q.addBindValue(curr_dt_string);
                if (q.exec())
                {

                }
                else
                {

                    qDebug() << q.lastQuery() << q.lastError();
                }
            }
            if (tag == "Z0G07") // 推沙节点
            {
                Q_ASSERT(list.length() > 7);

                q.prepare("INSERT INTO SQ_CURVE_NODE ("
                          "stcd ,"
                          "year ,"
                          "curve_id ,"
                          "node_id ,"
                          "sssc ,"
                          "csmsc ,"
                          "timestamp"
                          ") VALUES(?,?,?,?,?,?,?)");
                q.addBindValue(list[1]);
                q.addBindValue(list[2]);
                q.addBindValue(list[3]);
                q.addBindValue(list[4]);
                q.addBindValue(list[5]);
                q.addBindValue(list[6]);
                auto curr_dt_string = QDateTime::currentDateTime().toString(DEFAULT_DATETIME_FORMAT);
                q.addBindValue(curr_dt_string);
                if (q.exec())
                {

                }
                else
                {

                    qDebug() << q.lastQuery() << q.lastError();
                }
            }

            if (tag == "Z0G08") // 单沙
            {
                Q_ASSERT(list.length() > 8);

                q.prepare("INSERT INTO SQ_PROCESS ("
                          "stcd,"
                          "year,"
                          "row_id,"
                          "month ,"
                          "day,"
                          "time ,"
                          "concentration,"
                          "annotation,"
                          "timestamp"
                          ") VALUES(?,?,?,?,?,?,?,?,?)");
                q.addBindValue(list[1]);
                q.addBindValue(list[2]);
                q.addBindValue(list[3]);
                auto shortDt = DateTimeHelper::formalizeShortFormat(list[4]);
                q.addBindValue(shortDt.mid(0, 2));
                q.addBindValue(shortDt.mid(2, 2));
                q.addBindValue(shortDt.mid(4));
                q.addBindValue(list[5]);
                q.addBindValue(list[6]);
                auto datetime = DateTimeHelper::fromShortFormat(
                            list[4], list[2].toInt());
                q.addBindValue(datetime
                             .toString(DEFAULT_DATETIME_FORMAT));
                if (q.exec())
                {

                }
                else
                {

                    qDebug() << q.lastQuery() << q.lastError();
                }
            }

            if (tag == "Z0G09") // 水位
            {
                Q_ASSERT(list.length() > 8);

                q.prepare("INSERT INTO ZQ_PROCESS ("
                          "stcd ,"
                          "year ,"
                          "row_id ,"
                          "month ,"
                          "day,"
                          "time ,"
                          "stage ,"
                          "annotation ,"
                          "timestamp "
                          ") VALUES(?,?,?,?,?,?,?,?,?)");
                q.addBindValue(list[1]);
                q.addBindValue(list[2]);
                q.addBindValue(list[3]);
                auto shortDt = DateTimeHelper::formalizeShortFormat(list[4]);
                q.addBindValue(shortDt.mid(0, 2));
                q.addBindValue(shortDt.mid(2, 2));
                q.addBindValue(shortDt.mid(4));
                q.addBindValue(list[5]);
                q.addBindValue(list[6]);
                auto datetime = DateTimeHelper::fromShortFormat(
                            list[4], list[2].toInt());
                q.addBindValue(datetime
                             .toString(DEFAULT_DATETIME_FORMAT));
                if (q.exec())
                {

                }
                else
                {

                    qDebug() << q.lastQuery() << q.lastError();
                }
            }

        }
        q.exec("END;");
        qDebug() << line; // 最后一行

        progress.setValue(totalRecordCount);

        if (!db.commit())
        {
            db.rollback();
        }
        db.close();
    }
#endif
}

void Z0gFile::writeFile()
{
    if (_pfile == nullptr)
    {
        return;
    }
}

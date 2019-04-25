#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "z0gfile.h"

#include <QtDebug>
#include <QtWidgets>

#ifdef QT_SQL
#include <QtSql>
#endif

#include "managers/databasemanager.h"
#include "models/table.h"
#include "models/station.h"
#include "helpers/datetimehelper.h"


Z0gFile::Z0gFile(const QString& filename)
{
    _pfile = new QFile(filename);
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
        auto db = DatabaseManager::instance()->openDatabase();
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

        // 查找该年数据是否已经存在，存在则提示用户删除

        QString sqlSelectCount = "SELECT COUNT(*) FROM ";
        QString sqlWhere = " WHERE stcd = :stcd AND year = :year";
        QList<Table> tables;
        Station station(stcd);

        tables << Table("ZQ_PERIOD")
               << Table("SQ_PERIOD")
               << Table("ZQ_CURVE_NODE")
               << Table("SQ_CURVE_NODE")
               << Table("SQ_PROCESS")
               << Table("ZQ_PROCESS")
               << Table("ZQ_MEASUREMENT")
               << Table("SQ_MEASUREMENT")
               << Table("CS_SURVEY")
               << Table("CS_VERTICAL");


        bool yesToAll = false;
        bool noToAll = false;
        for (auto table : tables)
        {
            q.prepare(sqlSelectCount + table.tableName() + sqlWhere);
            q.bindValue(":stcd", stcd);
            q.bindValue(":year", year);

            if (q.exec())
            {
                while (q.next())
                {
                    int count = q.value(0).toInt();
                    qDebug() << count << q.lastQuery();
                    if (count > 0)
                    {
                        QMessageBox box(QMessageBox::Question,
                                        QObject::tr("疑问"),
                                        QObject::tr("数据库中存在%1站%2年%3数据，为保证数据唯一性最好清空，"
                                                    "否则可能有重复数据。")
                                        .arg(station.getDisplayName())
                                        .arg(year)
                                        .arg(table.getDisplayName()),
                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort |
                                        QMessageBox::YesToAll | QMessageBox::NoToAll);
                        box.setButtonText(QMessageBox::Yes, QObject::tr("删除%1数据")
                                          .arg(table.getDisplayName()));
                        box.setButtonText(QMessageBox::No, QObject::tr("保留%1数据")
                                          .arg(table.getDisplayName()));
                        box.setButtonText(QMessageBox::YesToAll, QObject::tr("删除该站该年全部数据"));
                        box.setButtonText(QMessageBox::NoToAll, QObject::tr("保留该站该年全部数据"));
                        box.setButtonText(QMessageBox::Abort, QObject::tr("终止导入"));

                        int selectedBtn = -1;
                        if (!(yesToAll || noToAll))
                        {
                            selectedBtn = box.exec();
                        }

                        if (QMessageBox::Yes == selectedBtn || yesToAll)
                        {
                            QString sqlDelete = "DELETE FROM ";
                            QString sqlWhere = " WHERE stcd = :stcd AND year = :year";
                            QSqlQuery q2(db);
                            q2.prepare(sqlDelete + table.tableName() + sqlWhere);
                            q2.bindValue(":stcd", stcd);
                            q2.bindValue(":year", year);
                            bool ret = q2.exec();
                            //                        qDebug() << ret << q2.lastQuery() << q2.lastError();
                        }
                        else if (QMessageBox::YesToAll == selectedBtn)
                        {
                            yesToAll = true;
                        }
                        else if (QMessageBox::No == selectedBtn || noToAll)
                        {
                            continue;
                        }
                        else if (QMessageBox::NoToAll == selectedBtn)
                        {
                            noToAll = true;
                        }
                        else if (QMessageBox::Abort == selectedBtn)
                        {
                            return;
                        }
                    }
                }
            }
            else
            {
                qDebug() << q.lastQuery() << q.lastError();
            }
        }
        qDebug() << "duplication check finished!";
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
                          "time,"
                          "concentration,"
                          "annotation,"
                          "timestamp"
                          ") VALUES(?,?,?,?,?,?,?)");
                q.addBindValue(list[1]);
                q.addBindValue(list[2]);
                q.addBindValue(list[3]);
                q.addBindValue(list[4]);
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
                          "time ,"
                          "stage ,"
                          "annotation ,"
                          "timestamp "
                          ") VALUES(?,?,?,?,?,?,?)");
                q.addBindValue(list[1]);
                q.addBindValue(list[2]);
                q.addBindValue(list[3]);
                q.addBindValue(list[4]);
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

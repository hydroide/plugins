#include "zbfile.h"

#include <QMessageBox>

ZBFile::ZBFile(const QString &filepath)
{
    _pfile = new QFile(filepath);
}

ZBFile::~ZBFile()
{

}

bool ZBFile::checkDuplication(QSqlDatabase db, const QString &stcd, int year, QList<Table> tables)
{
    // 查找该年数据是否已经存在，存在则提示用户删除
    QSqlQuery q(db);

    QString sqlSelectCount = "SELECT COUNT(*) FROM ";
    QString sqlWhere = " WHERE stcd = :stcd AND year = :year";
    Station station(stcd);

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
                        return false;
                    }
                }
            }
        }
        else
        {
            qDebug() << q.lastQuery() << q.lastError();
        }
    }
    return true;
}

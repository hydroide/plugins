#ifndef ZBFILE_H
#define ZBFILE_H

#include <QtCore>

#include <QtSql>

#include "models/table.h"
#include "models/station.h"

class ZBFile
{
public:
    ZBFile(const QString &filepath);
    virtual ~ZBFile();

    virtual void readFile() = 0;
    virtual void writeFile() = 0;


protected:
    bool checkDuplication(QSqlDatabase db, const QString &stcd, int year, QList<Table> tables);

    QString _filename;
    QFile *_pfile{nullptr};
};

#endif // ZBFILE_H

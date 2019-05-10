#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef Z0GSSBPLUGIN_H
#define Z0GSSBPLUGIN_H

#include <QtPlugin>
#include <interfaces/fileimportinterface.h>
#include <interfaces/databaseinterface.h>

class Z0gSsbPlugin : public QObject, FileImportInterface, DatabaseInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hwsdsw.HydroCurve.FileImportInterface" FILE "z0gssb.json")
    Q_INTERFACES(FileImportInterface)
    Q_INTERFACES(DatabaseInterface)

    // FileImportInterface interface
public:
    QStringList extensions();
    QString description();
    void import(const QString &filepath);

    // DatabaseInterface interface
public:
    void setDatabase(QSqlDatabase db);
    static QSqlDatabase DB;
};

#endif // Z0GSSBPLUGIN_H

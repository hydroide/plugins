#include "z0gssbplugin.h"

#include "z0gfile.h"
#include "ssbfile.h"

QSqlDatabase Z0gSsbPlugin::DB;

QStringList Z0gSsbPlugin::extensions()
{
    return QStringList() << "Z0G" << "SSB";
}

QString Z0gSsbPlugin::description()
{
    return tr("北方片整编数据文件");
}

void Z0gSsbPlugin::import(const QString &filepath)
{
    if (filepath.endsWith("Z0G", Qt::CaseSensitivity::CaseInsensitive)) {
        Z0gFile file(filepath);
        file.readFile();
    } else if (filepath.endsWith("SSB", Qt::CaseSensitivity::CaseInsensitive)) {
        SsbFile file(filepath);
        file.readFile();
    }
}

void Z0gSsbPlugin::setDatabase(QSqlDatabase db)
{
    Z0gSsbPlugin::DB = db;
}

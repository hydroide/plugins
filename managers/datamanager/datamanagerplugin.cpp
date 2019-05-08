#include "datamanagerplugin.h"

#include "datawindow.h"

QSqlDatabase DB;

void DataManagerPlugin::setDatabase(QSqlDatabase db)
{
    DB = db;
}

QWidget *DataManagerPlugin::create(QWidget *parent)
{
    return new DataWindow(parent);
}

QString DataManagerPlugin::name()
{
    return tr("数据编辑器");
}

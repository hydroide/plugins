#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
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

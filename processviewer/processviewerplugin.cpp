#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "processviewerplugin.h"

#include "processviewer.h"

SpDataProviderInterface g_dataProvider{nullptr};

ProcessViewerPlugin::ProcessViewerPlugin(QObject *parent) :
    QObject(parent)
{
}

QWidget *ProcessViewerPlugin::create(QWidget *parent)
{
    return new ProcessViewer(parent);
}

QString ProcessViewerPlugin::name()
{
    return tr("过程查看器");
}

void ProcessViewerPlugin::setDataProvider(SpDataProviderInterface dataProvider)
{
    g_dataProvider = dataProvider;
}

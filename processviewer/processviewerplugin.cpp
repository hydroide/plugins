#include "processviewerplugin.h"

#include "processviewer.h"

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

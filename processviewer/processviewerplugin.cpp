#include "processviewerplugin.h"

#include "processviewer.h"

SpProject g_project{nullptr};

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

void ProcessViewerPlugin::setProject(SpProject project)
{
    g_project = project;
}

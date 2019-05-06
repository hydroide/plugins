#ifndef PROCESSVIEWERPLUGIN_H
#define PROCESSVIEWERPLUGIN_H

#include <interfaces/viewerinterface.h>
#include <interfaces/projectinterface.h>

class ProcessViewerPlugin : public QObject, public ViewerInterface, public ProjectInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hwsdsw.HydroCurve.ViewerInterface" FILE "processviewer.json")
    Q_INTERFACES(ViewerInterface)
    Q_INTERFACES(ProjectInterface)

public:
    ProcessViewerPlugin(QObject *parent = nullptr);

    // ViewerInterface interface
public:
    QWidget *create(QWidget *parent);
    QString name();

    // ProjectInterface interface
public:
    void setProject(SpProject project);
};

#endif // PROCESSVIEWERPLUGIN_H

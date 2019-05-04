#ifndef PROCESSVIEWERPLUGIN_H
#define PROCESSVIEWERPLUGIN_H

#include <viewerinterface.h>

class ProcessViewerPlugin : public QObject, public ViewerInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hwsdsw.HydroCurve.ViewerInterface" FILE "processviewer.json")
    Q_INTERFACES(ViewerInterface)

public:
    ProcessViewerPlugin(QObject *parent = 0);

    // ViewerInterface interface
public:
    QWidget *create(QWidget *parent);
    QString name();
};

#endif // PROCESSVIEWERPLUGIN_H

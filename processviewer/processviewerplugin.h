#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef PROCESSVIEWERPLUGIN_H
#define PROCESSVIEWERPLUGIN_H

#include <interfaces/viewerinterface.h>

#include <interfaces/dataaccessorinterface.h>

class ProcessViewerPlugin : public QObject, public ViewerInterface, DataAccessorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hwsdsw.HydroCurve.ViewerInterface" FILE "processviewer.json")
    Q_INTERFACES(ViewerInterface)
    Q_INTERFACES(DataAccessorInterface)

public:
    ProcessViewerPlugin(QObject *parent = nullptr);

    // ViewerInterface interface
public:
    QWidget *create(QWidget *parent);
    QString name();

    // DataAccessorInterface interface
public:
    void setDataProvider(SpDataProviderInterface);
};

#endif // PROCESSVIEWERPLUGIN_H

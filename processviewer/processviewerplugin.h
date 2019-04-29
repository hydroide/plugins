#ifndef PROCESSVIEWERPLUGIN_H
#define PROCESSVIEWERPLUGIN_H

#include <QtCore>

class ProcessViewerPlugin : public QObject
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "processviewer.json")
#endif // QT_VERSION >= 0x050000

public:
    ProcessViewerPlugin(QObject *parent = 0);
};

#endif // PROCESSVIEWERPLUGIN_H

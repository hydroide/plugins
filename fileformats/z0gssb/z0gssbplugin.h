#ifndef Z0GSSBPLUGIN_H
#define Z0GSSBPLUGIN_H

#include <QtPlugin>
#include <fileimportinterface.h>

class Z0gSsbPlugin : public QObject, FileImportInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hwsdsw.HydroCurve.FileImportInterface" FILE "z0gssb.json")
    Q_INTERFACES(FileImportInterface)

    // FileImportInterface interface
public:
    QStringList extensions();
    QString description();
    void import(const QString &filepath);
};

#endif // Z0GSSBPLUGIN_H

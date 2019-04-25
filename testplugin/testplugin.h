#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include "testinterface.h"

class TestPlugin : public QObject, TestInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hwsdsw.HydroCurve.TestInterface" FILE "testplugin.json")
    Q_INTERFACES(TestInterface)

public:
    QString echo(const QString &message) override;
};

#endif

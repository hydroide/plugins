#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include "interfaces/testinterface.h"

class TestPlugin : public QObject, TestInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hwsdsw.HydroCurve.TestInterface" FILE "testplugin.json")
    Q_INTERFACES(TestInterface)

public:
    QString echo(const QString &message) override;
};

#endif

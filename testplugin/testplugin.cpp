#include "testplugin.h"

QString TestPlugin::echo(const QString &message)
{
    return message + " processed by plugin";
}

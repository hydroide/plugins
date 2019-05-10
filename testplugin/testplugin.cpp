#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "testplugin.h"

QString TestPlugin::echo(const QString &message)
{
    return message + " processed by plugin";
}

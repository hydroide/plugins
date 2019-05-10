#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef PROJECTMANAGERPLUGIN_H
#define PROJECTMANAGERPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include "interfaces/projectinterface.h"

class ProjectManagerPlugin : public QObject, ProjectInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.hwsdsw.HydroCurve.ProjectManagerInterface" FILE "projectmanager.json")
    Q_INTERFACES(ProjectInterface)

public:

    // ProjectInterface interface
public:
    void setProject(SpProject);
};

#endif

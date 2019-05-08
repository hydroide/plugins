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

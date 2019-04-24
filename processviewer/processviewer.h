#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef PROCESSVIEWER_H
#define PROCESSVIEWER_H

#include "viewer.h"

class ProcessViewer : public Viewer
{
    Q_OBJECT
public:
    explicit ProcessViewer(QWidget *parent = nullptr);

protected:
    void addViewToSplitter(QSplitter *splitter, ProcessChart *chart);

Q_SIGNALS:
    void selectionChanging(qreal startHour, qreal endHour);
    void selectionChanged(qreal startHour, qreal endHour);

    // Viewer interface
public:
    void loadToolBar();

    // QWidget interface
protected:
    void showEvent(QShowEvent *event);
    void updateContext();
};

#endif // PROCESSVIEWER_H

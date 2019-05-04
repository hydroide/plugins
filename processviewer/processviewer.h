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

    QWidget *_vbox10Widget;
    QVBoxLayout *_vbox10;
    QHBoxLayout *_hbox11;
    QWidget *_hbox11Widget;
    QSpacerItem *_spacer11;

    QLabel *_label_selectYear;
    QSpinBox *_sb_selectYear;
    QPushButton *_pb_selectYear;

    QLabel *_label_selectMonth;
    QSpinBox *_sb_selectMonth;
    QPushButton *_pb_selectMonth;

    QLabel *_label_startDateTime;
    QDateTimeEdit* _dte_startDateTime;

    QLabel *_label_endDateTime;
    QDateTimeEdit* _dte_endDateTime;

    QPushButton *_viewOption;
    QPushButton *_insertTo;
    QPushButton *_generate;

    QGridLayout *_grid;
    QWidget *_gridWidget;

    QSplitter *_splitter11;

    QPushButton *_buttonPlus1;
    ProcessXAxisWidget *_xAxis1;

    QList<View *> _views;
    QList<ProcessChart *> _processCharts;

    QToolBar *_toolBar;

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

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef DATAEDITOR_H
#define DATAEDITOR_H

#include <QtWidgets>

#include <QtSql>

#include "sqltablemodel.h"

class DataEditor : public QWidget
{
    Q_OBJECT

public:
    explicit DataEditor(const QString &tableName, QWidget *parent = nullptr);

private Q_SLOTS:
    void submit();
    void nextPage();
    void prevPage();

private:
    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *quitButton;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QDialogButtonBox *buttonBox;
    QLabel *stationLabel;
    QComboBox *stationComboBox;
    QLabel *yearLabel;
    QComboBox *yearComboBox;
    QLabel *currentPageLabel;

    SqlTableModel *model;
};

#endif // DATAEDITOR_H

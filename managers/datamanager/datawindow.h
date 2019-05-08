#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class DataWindow;
}

class DataWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataWindow(QWidget *parent = nullptr);
    ~DataWindow();

    void loadTable(const QString &tableName);

private Q_SLOTS:
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::DataWindow *ui;

    // QWidget interface
protected:
    void showEvent(QShowEvent *event);
};

#endif // DATAWINDOW_H

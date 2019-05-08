#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "datawindow.h"
#include "ui_datawindow.h"

#include "dataeditor.h"

#include "hcglobaldefs.h"

DataWindow::DataWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DataWindow)
{
    ui->setupUi(this);
}

DataWindow::~DataWindow()
{
    delete ui;
}

void DataWindow::loadTable(const QString &tableName)
{
    auto widget = this->centralWidget();
    delete widget;
    widget = new DataEditor(tableName, this);
    this->setCentralWidget(widget);
}

void DataWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current) {
        loadTable(current->data(ROLE_TABLE_NAME).toString());
    }
}

void DataWindow::showEvent(QShowEvent *event)
{
    ui->listWidget->clear();
//    for (auto tableName : DatabaseManager::instance()->getTables())
//    {
//       auto item = new QListWidgetItem(ui->listWidget);
//       item->setData(ROLE_TABLE_NAME, tableName);
//       QString displayName = tableName;
//       if (MAP_TABLE_DISPLAY_NAMES.contains(tableName))
//       {
//           displayName = MAP_TABLE_DISPLAY_NAMES[tableName];
//       }
//       item->setText(displayName);
//    }
    for (auto key: MAP_TABLE_DISPLAY_NAMES.keys()) {
        auto item = new QListWidgetItem(ui->listWidget);
        item->setData(ROLE_TABLE_NAME, key);
        item->setText(MAP_TABLE_DISPLAY_NAMES[key]);
    }
}

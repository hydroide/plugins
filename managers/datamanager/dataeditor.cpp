#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "dataeditor.h"

extern QSqlDatabase DB;

DataEditor::DataEditor(const QString &tableName, QWidget *parent)
    : QWidget(parent)
{
//    auto db = DatabaseManager::instance()->openDatabase();
    auto db = DB;
    model = new SqlTableModel(this, db);
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
//    model->setCountPerPage(100);
//    model->setHeaderData(0, Qt::Horizontal, tr("ID"));

    QTableView *view = new QTableView;
    view->setModel(model);
//    view->hideColumn(0);
//    view->hideColumn(1);
//    view->hideColumn(2);
//    view->hideColumn(model->columnCount() - 1);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    view->verticalHeader()->hide();
    view->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    submitButton = new QPushButton(tr("提交"));
    submitButton->setDefault(true);
    revertButton = new QPushButton(tr("还原"));
    quitButton = new QPushButton(tr("退出"));
    nextButton = new QPushButton(tr("下一页"));
    prevButton = new QPushButton(tr("上一页"));

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(submitButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    buttonBox->addButton(prevButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(nextButton, QDialogButtonBox::ActionRole);

    connect(submitButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(revertButton, SIGNAL(clicked()), model, SLOT(revertAll()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(nextPage()));
    connect(prevButton, SIGNAL(clicked()), this, SLOT(prevPage()));

    stationLabel = new QLabel(tr("测站"));
    stationComboBox = new QComboBox();
    yearLabel = new QLabel(tr("年份"));
    yearComboBox = new QComboBox();

    auto filterEdt = new QLineEdit();
    auto filterBtn = new QPushButton();

    connect(filterBtn, &QPushButton::clicked, [=] {
        model->setFilter(filterEdt->text()) ;
    });

    auto sortEdt = new QLineEdit();
    auto sortBtn = new QPushButton();

    connect(sortBtn, &QPushButton::clicked, [=] {
        model->setSort(model->record().indexOf(sortEdt->text()), Qt::AscendingOrder);
        model->select();
    });

    currentPageLabel = new QLabel("第1页");
    QHBoxLayout *lineLayout = new QHBoxLayout;
    lineLayout->addWidget(stationLabel);
    lineLayout->addWidget(stationComboBox);
    lineLayout->addWidget(yearLabel);
    lineLayout->addWidget(yearComboBox);
    lineLayout->addWidget(filterEdt);
    lineLayout->addWidget(filterBtn);
    lineLayout->addWidget(sortEdt);
    lineLayout->addWidget(sortBtn);
    lineLayout->addWidget(buttonBox);
    lineLayout->addWidget(currentPageLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(lineLayout);
    mainLayout->addWidget(view);
    setLayout(mainLayout);

//    setWindowTitle(tr("Cached Table"));
}

void DataEditor::submit()
{
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    } else {
        model->database().rollback();
        QMessageBox::warning(this, tr("保存失败"),
                             tr("数据库错误: %1")
                             .arg(model->lastError().text()));
    }
}

void DataEditor::nextPage()
{
    model->setCurrentPage(model->currentPage() + 1);
    model->select();
    currentPageLabel->setText(QString("第%1页").arg(model->currentPage() + 1));
}

void DataEditor::prevPage()
{
    if (model->currentPage() > 0) {
        model->setCurrentPage(model->currentPage() - 1);
        model->select();
    }
    currentPageLabel->setText(QString("第%1页").arg(model->currentPage() + 1));
}

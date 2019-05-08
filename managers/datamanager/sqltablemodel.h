#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QSqlTableModel>

class SqlTableModel : public QSqlTableModel
{
public:
    explicit SqlTableModel(QObject *parent = Q_NULLPTR, QSqlDatabase db = QSqlDatabase());
    virtual ~SqlTableModel();
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    int countPerPage() const;
    void setCountPerPage(int countPerPage);

    int currentPage() const;
    void setCurrentPage(int currentPage);

protected:
    QString selectStatement() const;
    QString orderByClause() const;
    QStringList _orderList;
    int _countPerPage {100};
    int _currentPage {0};

};

#endif // SQLTABLEMODEL_H

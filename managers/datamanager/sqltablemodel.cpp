#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "sqltablemodel.h"

#include <QDebug>

SqlTableModel::SqlTableModel(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{

}

SqlTableModel::~SqlTableModel()
{

}

QVariant SqlTableModel::headerData(int section,
                                   Qt::Orientation orientation,
                                   int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Vertical) {
            return section + _currentPage*_countPerPage + 1;
        }
    }
    return QSqlTableModel::headerData(section, orientation, role);
}

QString SqlTableModel::selectStatement() const
{
    return QSqlTableModel::selectStatement() +
            QString(" LIMIT %1 OFFSET %2")
            .arg(_countPerPage)
            .arg(_countPerPage*_currentPage);
}

QString SqlTableModel::orderByClause() const
{
    qDebug() << QSqlTableModel::orderByClause();
    return QSqlTableModel::orderByClause();
}

int SqlTableModel::currentPage() const
{
    return _currentPage;
}

void SqlTableModel::setCurrentPage(int currentPage)
{
    if (currentPage >= 0 &&
            ((currentPage > _currentPage && rowCount() == _countPerPage) ||
            currentPage < _currentPage)) {
        _currentPage = currentPage;
    }
}

int SqlTableModel::countPerPage() const
{
    return _countPerPage;
}

void SqlTableModel::setCountPerPage(int countPerPage)
{
    _countPerPage = countPerPage;
}

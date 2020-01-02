#ifndef DB_H
#define DB_H

#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
class QString;
class QSqlQuery;
QT_END_NAMESPACE

class DbManager
{
public:
    DbManager(const QString&);

    bool addDate(const QString&);
    bool removeDate(const QString&);
    bool editDate(const QString&, const QString&);
    QStringList showAllDate();
    QString fetchDate(const QString&);

private:
    QSqlDatabase p_db;
    QSqlQuery* query;
};
#endif

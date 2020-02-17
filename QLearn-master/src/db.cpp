#include <QtSql>
#include <QString>

#include "header/db.h"

bool executeQuery(QSqlQuery* const q)
{
    if(q->exec())
    {
        delete q;
        return 1;
    }else
    {
        qDebug() << "addDate error:  "
                 << q -> lastError();

        delete q;
        return 0;
    }
}

DbManager::DbManager(const QString& path)
{
   p_db = QSqlDatabase::addDatabase("QSQLITE");
   p_db.setDatabaseName(path);
   
   if (!p_db.open())
   {
      qDebug() << "Error: connection with database fail";
   }
   else
   {
      qDebug() << "Database: connection ok";

      if(!p_db.tables().contains("time",Qt::CaseInsensitive))
      {
        qDebug() << "Creating table";
        QSqlQuery q;
        q.prepare("create table time(id integer primary key, date varchar)");
        qDebug() << q.exec();
      }
   }
}

bool DbManager::addDate(const QString &date)
{
    query = new QSqlQuery;
    query -> prepare("INSERT INTO time(date) VALUES (:date)");
    query -> bindValue(":date", date);

    return executeQuery(query);
}

bool DbManager::removeDate(const QString &date)
{
    query = new QSqlQuery;
    query -> prepare("DELETE FROM time WHERE date = :date");
    query -> bindValue(":date", date);

    return executeQuery(query);
}


bool DbManager::editDate(const QString &dateSource, const QString &dateDestin)
{
    query = new QSqlQuery;
    query -> prepare("UPDATE time SET date = :dateSource WHERE date = :dateDestin");
    query -> bindValue(":dateSource", dateSource);
    query -> bindValue(":dateDestin", dateDestin);

    return executeQuery(query);
}

QStringList DbManager::showAllDate()
{
    query = new QSqlQuery("SELECT * FROM time");
    QSqlRecord tmp = query -> record();
    unsigned short int nameCol = static_cast<unsigned short int>(tmp.indexOf("date"));
    QStringList listAll;
    while(query -> next())
    {
        listAll.append(query -> value(nameCol).toString());
    }

    return listAll;
}

QString DbManager::fetchDate(const QString &date)
{
    query = new QSqlQuery;
    query -> prepare("SELECT * FROM time WHERE date = :date");
    query -> bindValue(":date", date);
    try { query -> exec(); } catch(std::exception &err) { qDebug() << err.what(); throw std::exception(err); }

    QSqlRecord tmp = query -> record();
    unsigned short int nameCol = static_cast<unsigned short int>(tmp.indexOf("date"));

    query -> next();
    return query -> value(nameCol).toString();
}

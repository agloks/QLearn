#include "header/dateparse.h"
#include <iostream>
#include <QDebug>
#include <QDate>

DateParse::DateParse()
{

}

DateParse::~DateParse()
{
    delete listToday;
}

std::unique_ptr<std::vector<QStringList>> toList(const QString &lhs, const QString &rhs)
{
    QStringList arrLhs = lhs.split(QRegExp("/|\\s"));
    QStringList arrRhs = rhs.split(QRegExp("/|\\s"));
    std::unique_ptr<std::vector<QStringList>> ptr(new std::vector<QStringList>);

    ptr->push_back(arrLhs); ptr->push_back(arrRhs);
    return ptr;
}

void DateParse::orderFull(QStringList &sl)
{
    std::for_each(sl.begin(), sl.end(), [&](QString &elem){
        QString tempDay = elem.split(QRegExp("/|\\s"))[0];
        QString tempYear = elem.split(QRegExp("/|\\s"))[2];
        elem.replace(6, 4, tempDay);
        elem.replace(0, 2, tempYear);
    });
    std::stable_sort(sl.begin(), sl.end(), [](const QString &lhs, const QString &rhs) {
        std::unique_ptr<std::vector<QStringList>> arrList = toList(lhs, rhs);

        int sumArrLhs = ((*arrList)[0][0] + (*arrList)[0][1] + (*arrList)[0][2]).toInt();
        int sumArrRhs = ((*arrList)[1][0] + (*arrList)[1][1] + (*arrList)[1][2]).toInt();

        return sumArrLhs > sumArrRhs;
    });

    qDebug() << sl;
}

void DateParse::adjustDateBr(QString& date)
{
    QStringList arrDate = date.split(" ");

    QMap<QString, QString> months;
    months["jan"] = "01";
    months["feb"] = "02";
    months["mar"] = "03";
    months["apr"] = "04";
    months["may"] = "05";
    months["jun"] = "06";
    months["jul"] = "07";
    months["aug"] = "08";
    months["sep"] = "09";
    months["oct"] = "10";
    months["nov"] = "11";
    months["dec"] = "12";

    arrDate[1].replace(0, 3, months[arrDate[1]]);
    date = QString("%1/%2/%3").arg(arrDate[3]).arg(arrDate[1]).arg(arrDate[2]);
}

void DateParse::setListToday(const QStringList &sl)
{
    listToday = new QStringList();
    QString dateToday = QDate::currentDate().toString().toLower();
    this -> adjustDateBr(dateToday);

    for(const QString daySl: sl)
    {
        std::unique_ptr<std::vector<QStringList>> arrList = toList(daySl, dateToday);
        int numberDaySl = ((*arrList)[0][0] + (*arrList)[0][1] + (*arrList)[0][2]).toInt();
        int numberDateToday = ((*arrList)[1][0] + (*arrList)[1][1] + (*arrList)[1][2]).toInt();
        if(std::abs(numberDaySl - numberDateToday) == 0)
        {
            listToday->push_back(daySl);
        }
    }

    qDebug() << *listToday;
}

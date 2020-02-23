#include "header/dateparse.h"
#include <iostream>
#include <QDebug>
#include <QDate>
#include <QTime>

DateParse::DateParse()
{

}

DateParse::~DateParse()
{
    delete listToday;
}

std::unique_ptr<std::vector<QStringList>> toList(const QString &lhs, const QString &rhs, const char *sep)
{
    QStringList arrLhs = lhs.split(QRegExp(sep));
    QStringList arrRhs = rhs.split(QRegExp(sep));
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
        std::unique_ptr<std::vector<QStringList>> arrList = toList(lhs, rhs, "/|\\s");

        int sumArrLhs = ((*arrList)[0][0] + (*arrList)[0][1] + (*arrList)[0][2]).toInt();
        int sumArrRhs = ((*arrList)[1][0] + (*arrList)[1][1] + (*arrList)[1][2]).toInt();

        return sumArrLhs > sumArrRhs;
    });

//    qDebug() << "DataParse::orderFull => \n" << sl;
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

template<typename T>
std::unique_ptr<T> getHourAndMinute(const T &args)
{
    std::unique_ptr<T> gtm(new T(args));
    return gtm;
}


template<typename T>
std::unique_ptr<T> getHourAndMinute(const int &arg1, const int &arg2)
{
    std::unique_ptr<T> gtm(new T(arg1, arg2));
    return gtm;
}

void orderByTime(QStringList &sl)
{
    /*
        sl => ("2020/02/19 23:00 moremore recent", "2020/02/19 20:00 more recent", "2020/02/19 13:00 less recent");
        expected in arrList =>  ("2020/02/19", "13", "00", "less", "recent")
    */
    int timeNow = QTime::currentTime().msecsSinceStartOfDay();
    int idx = 0;
    ulong minFound = ULONG_MAX;

    while(idx ++< sl.size())
    {
        std::unique_ptr<QString> hour = getHourAndMinute<QString>(sl[idx - 1].split(" ")[1].split(":")[0]);
        std::unique_ptr<QString> minute = getHourAndMinute<QString>(sl[idx - 1].split(" ")[1].split(":")[1]);
        std::unique_ptr<QTime> timeTemp = getHourAndMinute<QTime>(hour->toInt(), minute->toInt());
        int timeInsert = timeTemp->msecsSinceStartOfDay();

        ulong diff = (timeInsert >= timeNow) ? timeInsert - timeNow : 32765;

        if(diff <= minFound)
        {
            std::swap(sl[0], sl[idx - 1]);
            minFound = diff;
        };
    }

    std::stable_sort(sl.begin() + 1, sl.end(), [](const QString &lhs, const QString &rhs){
        std::unique_ptr<std::vector<QStringList>> arrList = toList(lhs, rhs, " |:");
        const int lhsInt = ((*arrList)[0][1] + (*arrList)[0][2]).toInt();
        const int rhsInt = ((*arrList)[1][1] + (*arrList)[1][2]).toInt();


        return lhsInt > rhsInt;
    });
}


void DateParse::setListToday(const QStringList &sl)
{
    listToday = new QStringList();
    QString dateToday = QDate::currentDate().toString().toLower();
    qDebug() << dateToday;
    this -> adjustDateBr(dateToday);

    for(const QString daySl: sl)
    {
        std::unique_ptr<std::vector<QStringList>> arrList = toList(daySl, dateToday, "/|\\s");
        int numberDaySl = ((*arrList)[0][0] + (*arrList)[0][1] + (*arrList)[0][2]).toInt();
        int numberDateToday = ((*arrList)[1][0] + (*arrList)[1][1] + (*arrList)[1][2]).toInt();
//        qDebug() << "numberDaySl = " << numberDaySl;
        qDebug() << "numberDayToday = " << (*arrList)[1];
        if(std::abs(numberDaySl - numberDateToday) == 0)
        {
            listToday->push_back(daySl);
        }
    }

    orderByTime(*listToday);

    qDebug() << "DataParse::setListToday =>\n" << *listToday;
}

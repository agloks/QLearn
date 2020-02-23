#ifndef DATEPARSE_H
#define DATEPARSE_H

class QStringList;
class QString;
template <typename T1, typename T2> class QMap;

class DateParse {
    protected:
        DateParse();
        ~DateParse();

        void virtual orderFull(QStringList &);

        void virtual setListToday(const QStringList &);
        void virtual adjustDateBr(QString &);

        QStringList* listToday;
        QMap<QString, QString>* months;
    private:
        void m_setMonthMap();
};

#endif // DATEPARSE_H

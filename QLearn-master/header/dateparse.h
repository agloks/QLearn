#ifndef DATEPARSE_H
#define DATEPARSE_H

class QStringList;
class QString;

class DateParse {
    protected:
        DateParse();
        ~DateParse();

        void virtual orderFull(QStringList &);
        void virtual setListToday(const QStringList &);
        void virtual adjustDateBr(QString &);
//        void orderByMonth();
        QStringList* listToday;
};

#endif // DATEPARSE_H

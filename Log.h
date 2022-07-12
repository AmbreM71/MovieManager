#ifndef LOG_H
#define LOG_H

#include <QList>
#include <QObject>
#include <QDate>
#include <QTime>


class Log : public QObject {

    Q_OBJECT

    private:
       QList<QString> m_log;

    public:
        Log();
        void append(QString s);
        void clear();
        QString getLog(int i);
        int size();

    signals:
        void logAppended();


};

#endif // LOG_H

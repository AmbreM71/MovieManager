#ifndef LOG_H
#define LOG_H

#include <QList>
#include <QObject>
#include <QDate>
#include <QTime>
#include <Structures.h>


class Log : public QObject {

    Q_OBJECT

    private:
       QList<LogElement> m_log;

    public:
        Log();
        void append(QString s, enum eLog criticity);
        void clear();
        LogElement getLog(int i);
        int size();

    signals:
        void logAppended();


};

#endif // LOG_H

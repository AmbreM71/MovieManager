#ifndef LOG_H
#define LOG_H

#include <QList>


class Log {

    private:
       QList<QString> m_log;

    public:
        Log();
        void append(QString s);
        void clear();
        QString getLog(int i);
        int size();
};

#endif // LOG_H

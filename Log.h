#ifndef LOG_H
#define LOG_H

#include <QList>
#include <QObject>


class Log : public QObject {

    Q_OBJECT

    private:
       QList<QString> m_log;
       std::tm* now;

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

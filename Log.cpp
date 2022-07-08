#include "Log.h"

Log::Log() {
}

void Log::append(QString s) {
    std::time_t t = std::time(0);
    now = std::localtime(&t);
    QString datetime = QString::number(now->tm_year+1900) + "-" + QString::number(now->tm_mon+1) + "-" + QString::number(now->tm_mday) + " " + QString::number(now->tm_hour) + ":" + QString::number(now->tm_min) + ":" + QString::number(now->tm_sec);
    m_log.append(datetime + " : " + s);
    emit logAppended();
}

void Log::clear() {
    m_log.clear();
}

QString Log::getLog(int i) {
    QList<QString>::iterator it = m_log.begin();
    std::advance(it, i);
    return *it;
}

int Log::size() {
    return m_log.size();
}

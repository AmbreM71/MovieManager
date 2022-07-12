#include "Log.h"

Log::Log() {

}

void Log::append(QString s) {
    QString datetime = QString::number(QDate::currentDate().year()) + "-" + QString::number(QDate::currentDate().month()) + "-" + QString::number(QDate::currentDate().day()) + " " + QString::number(QTime::currentTime().hour()) + ":" + QString::number(QTime::currentTime().minute()) + ":" + QString::number(QTime::currentTime().second());
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

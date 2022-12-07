#include "Log.h"

Log::Log() {

}

void Log::append(QString s, enum eLog criticity) {
    QString datetime = QString::number(QDate::currentDate().year()) + "-" + QString::number(QDate::currentDate().month()) + "-" + QString::number(QDate::currentDate().day()) + " " + QString::number(QTime::currentTime().hour()) + ":" + QString::number(QTime::currentTime().minute()) + ":" + QString::number(QTime::currentTime().second());
    struct LogElement elem;
    elem.string = datetime + tr(" : ") + s;
    elem.criticity = criticity;
    m_log.append(elem);
    emit logAppended();
}

void Log::clear() {
    m_log.clear();
}

LogElement Log::getLog(int i) {
    QList<LogElement>::iterator it = m_log.begin();
    std::advance(it, i);
    return *it;
}

int Log::size() {
    return m_log.size();
}

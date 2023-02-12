#ifndef CALENDARDIALOG_H
#define CALENDARDIALOG_H

#include <QDialog>
#include <QDate>
#include <QGridLayout>
#include <QLabel>
#include <QStringList>
#include <QSqlQuery>

#include "Common.h"

namespace Ui {
    class CalendarDialog;
}

class CalendarDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::CalendarDialog* m_ui;
        QDate m_selectedDate;
        int m_daysPerMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        QStringList m_monthLabel = {
            tr("Janvier"),
            tr("Février"),
            tr("Mars"),
            tr("Avril"),
            tr("Mai"),
            tr("Juin"),
            tr("Juillet"),
            tr("Août"),
            tr("Septembre"),
            tr("Octobre"),
            tr("Novembre"),
            tr("Décembre")
        };

        static int instances;

    public:
        explicit CalendarDialog(QWidget *parent = nullptr);
        ~CalendarDialog();

        void setDisplay();
        void setData();
        int getOffset();

        static int instancesCount();

    public slots:
        void previousButtonClicked();
        void nextButtonClicked();

};

#endif // CALENDARDIALOG_H

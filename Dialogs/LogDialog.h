#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>
#include "Log.h"

namespace Ui {
class LogDialog;
}

class LogDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::LogDialog* m_ui;
        Log* m_log;
        static int instances;

    public:

        explicit LogDialog(Log* log, QWidget *parent = nullptr);
        ~LogDialog();

        void fillList();
        static int instancesCount();

    public slots:
        void refreshLog();


};

#endif // LOGDIALOG_H

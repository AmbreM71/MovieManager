#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>

#include "Common.h"

namespace Ui {
class LogDialog;
}

class LogDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::LogDialog* m_ui;
        static int instances;

    public:

        explicit LogDialog(QWidget *parent = nullptr);
        ~LogDialog();

        void fillList();
        static int instancesCount();

    public slots:
        void refresh();


};

#endif // LOGDIALOG_H

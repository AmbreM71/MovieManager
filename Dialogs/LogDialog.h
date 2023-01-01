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
        enum eTheme* m_theme;
        static int instances;

    public:

        explicit LogDialog(Log* log,  enum eTheme* theme, QWidget *parent = nullptr);
        ~LogDialog();

        void fillList();
        static int instancesCount();

    public slots:
        void refresh();


};

#endif // LOGDIALOG_H

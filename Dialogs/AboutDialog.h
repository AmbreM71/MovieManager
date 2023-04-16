#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QDesktopServices>
#include <QtVersion>

namespace Ui {
    class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::AboutDialog* m_ui;
        static int instances;

    public:
        explicit AboutDialog(QWidget *parent = nullptr);
        ~AboutDialog();

        static int instancesCount();

    public slots:
        void redirectGithub();


};

#endif // ABOUTDIALOG_H

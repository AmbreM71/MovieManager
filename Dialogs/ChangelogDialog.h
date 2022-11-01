#ifndef CHANGELOGDIALOG_H
#define CHANGELOGDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
    class ChangelogDialog;
}

class ChangelogDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::ChangelogDialog* m_ui;
        static int m_instances;
        QNetworkAccessManager manager;
        QNetworkReply* response;

    public:
        explicit ChangelogDialog(QWidget *parent = nullptr);
        ~ChangelogDialog();

        static int instancesCount();

    public slots:
        void loadHTML();

};

#endif // CHANGELOGDIALOG_H

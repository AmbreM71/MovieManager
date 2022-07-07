#ifndef ADDVIEWDIALOG_H
#define ADDVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class AddViewDialog;
}

class AddViewDialog : public QDialog
{
    Q_OBJECT
    private:
        Ui::AddViewDialog *ui;


    public:
        explicit AddViewDialog(QWidget *parent = nullptr);
        QString getName();
        int getReleaseYear();
        QString getViewType();
        QString getViewDate();
        int getRating();
        ~AddViewDialog();


};

#endif // ADDVIEWDIALOG_H

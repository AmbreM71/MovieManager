#ifndef ADDVIEWDIALOG_H
#define ADDVIEWDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QPushButton>

namespace Ui {
class AddViewDialog;
}

class AddViewDialog : public QDialog
{
    Q_OBJECT
    private:
        Ui::AddViewDialog* m_ui;


    public:
        explicit AddViewDialog(QWidget *parent = nullptr);
        ~AddViewDialog();

        void FillMovieComboBox();

        QString getComboboxSelectedItem();
        QString getName();
        int getReleaseYear();
        QString getViewType();
        QString getViewDate();
        int getRating();
        int getEntries();
        bool isDateUnknown();

    public slots:
        void comboboxChanged();
        void toggleViewDateInput(int state);
        void checkValid();


};

#endif // ADDVIEWDIALOG_H

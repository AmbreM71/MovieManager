#ifndef ADDVIEWDIALOG_H
#define ADDVIEWDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

#include <ShlObj.h>

namespace Ui {
class AddViewDialog;
}

class AddViewDialog : public QDialog
{
    Q_OBJECT
    private:
        Ui::AddViewDialog* m_ui;
        QString m_posterPath;


    public:
        explicit AddViewDialog(QWidget *parent = nullptr);
        ~AddViewDialog();

        void FillMovieComboBox();

        QString getComboboxSelectedItem();
        QString getName();
        int getReleaseYear();
        QString getViewType();
        QString getViewDate();
        QString getPosterPath();
        int getRating();
        int getEntries();
        bool isDateUnknown();
        bool isTypeUnknown();

    public slots:
        void comboboxChanged();
        void toggleViewDateInput(int state);
        void toggleViewTypeInput(int state);
        void checkValid();
        void loadPoster(QString path = "");


};

#endif // ADDVIEWDIALOG_H

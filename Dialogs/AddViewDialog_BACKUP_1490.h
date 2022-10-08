#ifndef ADDVIEWDIALOG_H
#define ADDVIEWDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QPushButton>
<<<<<<< HEAD
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
=======
#include <QList>
>>>>>>> tags

namespace Ui {
class AddViewDialog;
}

class AddViewDialog : public QDialog
{
    Q_OBJECT
    private:
        Ui::AddViewDialog* m_ui;
<<<<<<< HEAD
        QString m_posterPath;
=======
        QList<QString>* m_tags;
>>>>>>> tags


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
        QList<QString>* getTags();

    public slots:
        void comboboxChanged();
        void toggleViewDateInput(int state);
        void toggleViewTypeInput(int state);
        void checkValid();
<<<<<<< HEAD
        void loadPoster(QString path = "");
=======
        void addTag();
>>>>>>> tags


};

#endif // ADDVIEWDIALOG_H

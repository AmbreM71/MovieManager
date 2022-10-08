#ifndef EDITMOVIEDIALOG_H
#define EDITMOVIEDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QDate>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

#include <Common.h>

namespace Ui {
class EditMovieDialog;
}

class EditMovieDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::EditMovieDialog* m_ui;
        QString* m_ID;
        bool m_edited = false;
        bool m_newPoster = false;
        QString m_posterPath;

    public:
        explicit EditMovieDialog(QString ID, QWidget *parent = nullptr);
        ~EditMovieDialog();

        bool edited();
        bool newPoster();
        QString getMovieName();
        QString getReleaseYear();
        QString getPosterPath();
        int getRating();
        int getEntries();

    public slots:
        void loadPoster(QString path = "");


};

#endif // EDITMOVIEDIALOG_H

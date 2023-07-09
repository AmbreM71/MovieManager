#ifndef EDITMOVIEDIALOG_H
#define EDITMOVIEDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QDate>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

#include <Common.h>
#include <Tag.h>

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
        QList<QString>* m_tags;

    public:
        explicit EditMovieDialog(QString ID, QWidget *parent = nullptr);
        ~EditMovieDialog();

        bool edited();
        bool newPoster();
        QString getMovieName();
        QString getReleaseYear();
        QString getPosterPath();
        int getRating();
        QList<QString>* getTags();

    public slots:
        void loadPoster(QString path = "");
        void addTag();

        void clickedTag(Tag* tag);
        void mouseEnteredTag(Tag* tag);
        void mouseLeftTag(Tag* tag);


};

#endif // EDITMOVIEDIALOG_H

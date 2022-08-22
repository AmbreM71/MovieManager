#ifndef EDITMOVIEDIALOG_H
#define EDITMOVIEDIALOG_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class EditMovieDialog;
}

class EditMovieDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::EditMovieDialog* m_ui;
        QTableWidget* m_table;
        bool m_edited = false;

    public:
        explicit EditMovieDialog(QTableWidget* table, QWidget *parent = nullptr);
        ~EditMovieDialog();

        bool edited();
        QString getMovieName();
        QString getReleaseYear();
        int getRating();
        int getEntries();


};

#endif // EDITMOVIEDIALOG_H

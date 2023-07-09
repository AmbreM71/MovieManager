#ifndef ADDVIEWDIALOG_H
#define ADDVIEWDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

#include <Common.h>
#include <Tag.h>

namespace Ui {
class AddViewDialog;
}

class AddViewDialog : public QDialog
{
    Q_OBJECT
    private:
        Ui::AddViewDialog* m_ui;
        QList<QString>* m_tags;
        QString m_posterPath;

    public:
        explicit AddViewDialog(QWidget *parent, int nMovieID = -1);
        ~AddViewDialog();

        void FillMovieComboBox();

        QString getComboboxSelectedItem();
        QString getName();
        int getReleaseYear();
        int getViewType();
        QDate getViewDate();
        QString getPosterPath();
        int getRating();
        QList<QString>* getTags();
        bool isDateUnknown();
        bool isTypeUnknown();

    public slots:
        void comboboxChanged();
        void toggleViewDateInput(int state);
        void toggleViewTypeInput(int state);
        void checkValid();
        void loadPoster(QString path = "");
        void addTag();

        void clickedTag(Tag* tag);
        void mouseEnteredTag(Tag* tag);
        void mouseLeftTag(Tag* tag);

};

#endif // ADDVIEWDIALOG_H

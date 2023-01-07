#ifndef ADDVIEWDIALOG_H
#define ADDVIEWDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QSettings>

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

        QSettings* m_settings;

    public:
        explicit AddViewDialog(QWidget *parent, QSettings* settings);
        ~AddViewDialog();

        void FillMovieComboBox();

        QString getComboboxSelectedItem();
        QString getName();
        int getReleaseYear();
        int getViewType();
        QDate getViewDate();
        QString getPosterPath();
        int getRating();
        int getEntries();
        bool isDateUnknown();
        bool isTypeUnknown();
        bool isEntriesUnknown();
        QList<QString>* getTags();

    public slots:
        void comboboxChanged();
        void toggleViewDateInput(int state);
        void toggleViewTypeInput(int state);
        void toggleEntriesInput(int state);
        void checkValid();
        void loadPoster(QString path = "");
        void addTag();

        void clickedTag(Tag* tag);
        void mouseEnteredTag(Tag* tag);
        void mouseLeftTag(Tag* tag);

};

#endif // ADDVIEWDIALOG_H

#ifndef ADDVIEWDIALOG_H
#define ADDVIEWDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QKeyEvent>
#include <QCompleter>

#include <Common.h>
#include <Tag.h>
#include "TagsScrollArea.h"

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
        QList<QWidget*>* m_customColumnsInputList;
        QList<QString>* m_customColumnsNameList;
        QList<QCheckBox*> m_customColumnsUnknownCheckBoxList;
        TagsScrollArea* m_tagsScrollArea;
        QStringList m_sMovieList;

        bool eventFilter(QObject *obj, QEvent *event);

    public:
        explicit AddViewDialog(QWidget *parent, int nMovieID = -1);
        ~AddViewDialog();

        QStringList GetMovieList();

        bool IsSearchedMovieAnExistingMovie();
        QString getName();
        int getReleaseYear();
        int getViewType();
        QDate getViewDate();
        QString getPosterPath();
        int getRating();
        QList<QString>* getTags();
        bool isDateUnknown();
        bool isTypeUnknown();
        QList<QWidget*>* getCustomColumnsInputList();
        QList<QString>* getCustomColumnsNameList();
        QList<QCheckBox*> getCustomColumnsUnknownCheckBoxList();
        QString GetSearchedMovieText();

    public slots:
        void MovieSearchChanged();
        void toggleViewDateInput(int state);
        void toggleViewTypeInput(int state);
        void checkValid();
        void SelectPoster();
        void addTag();
        void ToggleWidgetState(int state, QWidget* widget);

        void clickedTag(Tag* tag);
        void mouseEnteredTag(Tag* tag);
        void mouseLeftTag(Tag* tag);

};

#endif // ADDVIEWDIALOG_H

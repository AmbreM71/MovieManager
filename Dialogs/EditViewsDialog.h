#ifndef EDITVIEWSDIALOG_H
#define EDITVIEWSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QPushButton>
#include <QMenu>

#include "Log.h"
#include "Enums.h"
#include "EditViewDialog.h"

namespace Ui {
class EditViewsDialog;
}

class EditViewsDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::EditViewsDialog* m_ui;
        int* m_ID;
        Log* m_log;
        bool m_edited = false;
        int* m_theme;

    public:
        explicit EditViewsDialog(int* ID, Log* log, int* theme, QWidget *parent = nullptr);
        ~EditViewsDialog();

        void fillTable();
        bool edited();

    public slots:
        void customMenuRequested(QPoint pos);
        void deleteView();
        void editView();


};

#endif // EDITVIEWSDIALOG_H

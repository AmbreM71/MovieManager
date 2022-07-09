#ifndef EDITVIEWSDIALOG_H
#define EDITVIEWSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QPushButton>
#include <QMenu>

#include "Log.h"

namespace Ui {
class EditViewsDialog;
}

class EditViewsDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::EditViewsDialog* m_ui;
        QTableWidget* m_MainWindowTable;
        Log* m_log;
        bool m_edited = false;

    public:
        explicit EditViewsDialog(QTableWidget* table, Log* log, QWidget *parent = nullptr);
        ~EditViewsDialog();

        void fillTable();
        bool edited();

    public slots:
        void customMenuRequested(QPoint pos);
        void deleteView();


};

#endif // EDITVIEWSDIALOG_H

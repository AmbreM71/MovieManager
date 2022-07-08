#ifndef EDITVIEWSDIALOG_H
#define EDITVIEWSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QSqlQuery>
#include <QSqlError>

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

    public:
        explicit EditViewsDialog(QTableWidget* table, Log* log, QWidget *parent = nullptr);
        ~EditViewsDialog();


};

#endif // EDITVIEWSDIALOG_H

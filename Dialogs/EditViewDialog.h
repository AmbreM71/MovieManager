#ifndef EDITVIEWDIALOG_H
#define EDITVIEWDIALOG_H

#include <QDialog>
#include <QDate>
#include <QTableWidget>

#include "Common.h"
#include "Enums.h"

namespace Ui {
    class EditViewDialog;
}

class EditViewDialog : public QDialog {
    Q_OBJECT

    private:
        Ui::EditViewDialog* m_ui;
        QString* m_DateFormat;

    public:
        explicit EditViewDialog(QTableWidget* table, QString* dateFormat, QWidget* parent = nullptr);
        ~EditViewDialog();

        QDate getViewDate();
        int getViewType();
        bool isDateUnknown();
        bool isTypeUnknown();

   public slots:
        void toggleViewDateInput(int state);
        void toggleViewTypeInput(int state);

};

#endif // EDITVIEWDIALOG_H

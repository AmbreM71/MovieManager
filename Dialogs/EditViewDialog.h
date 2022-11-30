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

    public:
        explicit EditViewDialog(QTableWidget* table, QWidget* parent = nullptr);
        ~EditViewDialog();

        QString getViewDate();
        int getViewType();
        bool isDateUnknown();
        bool isTypeUnknown();

   public slots:
        void toggleViewDateInput(int state);
        void toggleViewTypeInput(int state);

};

#endif // EDITVIEWDIALOG_H

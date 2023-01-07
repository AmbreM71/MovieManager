#ifndef EDITVIEWDIALOG_H
#define EDITVIEWDIALOG_H

#include <QDialog>
#include <QDate>
#include <QTableWidget>
#include <QSettings>

#include "Common.h"
#include "Enums.h"

namespace Ui {
    class EditViewDialog;
}

class EditViewDialog : public QDialog {
    Q_OBJECT

    private:
        Ui::EditViewDialog* m_ui;
        QSettings* m_settings;

    public:
        explicit EditViewDialog(QTableWidget* table, QSettings* settings, QWidget* parent = nullptr);
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

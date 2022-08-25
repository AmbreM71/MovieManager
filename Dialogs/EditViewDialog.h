#ifndef EDITVIEWDIALOG_H
#define EDITVIEWDIALOG_H

#include <QDialog>
#include <QDate>
#include <QTableWidget>

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
        QString getViewType();
        bool isDateUnknown();

   public slots:
        void toggleViewDateInput(int state);

};

#endif // EDITVIEWDIALOG_H

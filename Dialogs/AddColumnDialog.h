#ifndef ADDCOLUMNDIALOG_H
#define ADDCOLUMNDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <Structures.h>

namespace Ui {
    class AddColumnDialog;
}

class AddColumnDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::AddColumnDialog* m_ui;
        struct stColumn m_stColumn;

    public:
        explicit AddColumnDialog(QWidget *parent = nullptr);
        ~AddColumnDialog();
        stColumn* getColumn();

    public slots:
        void TypeChanged(QString sText);
        void DecimalsPrecisionChanged(int nPrecision);
        void IsColumnValid();
        void Validate();
};

#endif // ADDCOLUMNDIALOG_H

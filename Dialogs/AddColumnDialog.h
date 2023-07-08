#ifndef ADDCOLUMNDIALOG_H
#define ADDCOLUMNDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QStringList>
#include "Structures.h"
#include "Common.h"

namespace Ui {
    class AddColumnDialog;
}

class AddColumnDialog : public QDialog
{
    Q_OBJECT

    private:
        Ui::AddColumnDialog* m_ui;
        struct stColumn m_stColumn;
        QStringList sColumnNameList;

    public:
        explicit AddColumnDialog(QWidget *parent = nullptr, struct stColumn* stColumnToEdit = nullptr);
        ~AddColumnDialog();
        stColumn* getColumn();

    public slots:
        void TypeChanged(QString sText);
        void DecimalsPrecisionChanged(int nPrecision);
        void IsColumnValid();
        void Validate();
};

#endif // ADDCOLUMNDIALOG_H

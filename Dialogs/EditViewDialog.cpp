#include "EditViewDialog.h"
#include "ui_EditViewDialog.h"

EditViewDialog::EditViewDialog(QTableWidget* table, QWidget* parent) : QDialog(parent) {

    m_ui = new Ui::EditViewDialog;
    m_ui->setupUi(this);

    QObject::connect(m_ui->UnknownViewDateInput, SIGNAL(stateChanged(int)), this, SLOT(toggleViewDateInput(int)));

    QString viewDate = table->item(table->currentRow(),1)->text();
    QString support = table->item(table->currentRow(),2)->text();
    if(viewDate == "?") {
        m_ui->UnknownViewDateInput->setChecked(true);
    }
    else {
        int year = viewDate.sliced(0,4).toInt();
        int month = viewDate.sliced(5,2).toInt();
        int day = viewDate.sliced(8,2).toInt();
        m_ui->ViewDateInput->setDate(QDate(year, month, day));
    }

    m_ui->SupportInput->setCurrentText(support);

}

EditViewDialog::~EditViewDialog() {
    delete m_ui;
}

QString EditViewDialog::getViewDate() {
    QString year = QString::number(m_ui->ViewDateInput->date().year());
    QString month = QString::number(m_ui->ViewDateInput->date().month());
    QString day = QString::number(m_ui->ViewDateInput->date().day());

    QString s = year+"-";
    if(month.length()==1) {
        s.append("0");
    }
    s.append(month+"-");
    if(day.length()==1) {
        s.append("0");
    }
    s.append(day);
    return s;
}

QString EditViewDialog::getViewType() {
    return m_ui->SupportInput->currentText();
}

void EditViewDialog::toggleViewDateInput(int state) {
    if(m_ui->UnknownViewDateInput->isChecked()) {
        m_ui->ViewDateInput->setEnabled(false);
    }
    else {
        m_ui->ViewDateInput->setEnabled(true);
    }
}

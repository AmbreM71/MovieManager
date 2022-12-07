#include "EditViewDialog.h"
#include "ui_EditViewDialog.h"

EditViewDialog::EditViewDialog(QTableWidget* table, QWidget* parent) : QDialog(parent) {

    m_ui = new Ui::EditViewDialog;
    m_ui->setupUi(this);

    m_ui->ViewDateInput->setDate(QDate::currentDate());

    QObject::connect(m_ui->UnknownViewDateInput, SIGNAL(stateChanged(int)), this, SLOT(toggleViewDateInput(int)));
    QObject::connect(m_ui->UnknownViewTypeInput, SIGNAL(stateChanged(int)), this, SLOT(toggleViewTypeInput(int)));

    QString viewDate = table->item(table->currentRow(),1)->text();
    if(viewDate == "?") {
        m_ui->UnknownViewDateInput->setChecked(true);
    }
    else {
        int year = viewDate.sliced(0,4).toInt();
        int month = viewDate.sliced(5,2).toInt();
        int day = viewDate.sliced(8,2).toInt();
        m_ui->ViewDateInput->setDate(QDate(year, month, day));
    }

    for(int viewType = 0 ; viewType < eViewType::MaxViewType ; viewType++) {
        m_ui->ViewTypeInput->addItem(Common::viewTypeToQString((enum eViewType)viewType));
    }

    int viewType = Common::QStringToViewType(table->item(table->currentRow(),2)->text());
    if(viewType == eViewType::Unknown) {
        m_ui->UnknownViewTypeInput->setChecked(true);
    }
    else {
        m_ui->ViewTypeInput->setCurrentText(Common::viewTypeToQString((enum eViewType)viewType));
    }



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

int EditViewDialog::getViewType() {
    return Common::QStringToViewType(m_ui->ViewTypeInput->currentText());
}

bool EditViewDialog::isDateUnknown() {
    return m_ui->UnknownViewDateInput->isChecked();
}

bool EditViewDialog::isTypeUnknown() {
    return m_ui->UnknownViewTypeInput->isChecked();
}

void EditViewDialog::toggleViewDateInput(int state) {
    if(m_ui->UnknownViewDateInput->isChecked()) {
        m_ui->ViewDateInput->setEnabled(false);
    }
    else {
        m_ui->ViewDateInput->setEnabled(true);
    }
}

void EditViewDialog::toggleViewTypeInput(int state) {
    if(m_ui->UnknownViewTypeInput->isChecked()) {
        m_ui->ViewTypeInput->setEnabled(false);
    }
    else {
        m_ui->ViewTypeInput->setEnabled(true);
    }
}

#include "EditViewDialog.h"
#include "ui_EditViewDialog.h"

EditViewDialog::EditViewDialog(QTableWidget* table, QSettings* settings, QWidget* parent) : QDialog(parent) {

    m_ui = new Ui::EditViewDialog;
    m_ui->setupUi(this);
    this->setWindowIcon(QIcon(":/assets/Assets/Icons/Dark/edit.png"));
    m_settings = settings;

    m_ui->ViewDateInput->setDate(QDate::currentDate());
    m_ui->ViewDateInput->setDisplayFormat(m_settings->value("dateFormat").toString());

    QObject::connect(m_ui->UnknownViewDateInput, SIGNAL(stateChanged(int)), this, SLOT(toggleViewDateInput(int)));
    QObject::connect(m_ui->UnknownViewTypeInput, SIGNAL(stateChanged(int)), this, SLOT(toggleViewTypeInput(int)));

    QString viewDate = table->item(table->currentRow(),1)->text();
    if(viewDate == "?") {
        m_ui->UnknownViewDateInput->setChecked(true);
    }
    else {
        int year, month, day;
        if(m_settings->value("dateFormat").toString() == "yyyy-MM-dd") {
            year = viewDate.sliced(0,4).toInt();
            month = viewDate.sliced(5,2).toInt();
            day = viewDate.sliced(8,2).toInt();
        }
        else if (m_settings->value("dateFormat").toString() == "dd/MM/yyyy") {
            day = viewDate.sliced(0,2).toInt();
            month = viewDate.sliced(3,2).toInt();
            year = viewDate.sliced(6,4).toInt();
        }
        else /*if (m_settings->value("dateFormat").toString() == "MM/dd/yyyy")*/ {
            month = viewDate.sliced(0,2).toInt();
            day = viewDate.sliced(3,2).toInt();
            year = viewDate.sliced(6,4).toInt();
        }
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

QDate EditViewDialog::getViewDate() {
    return m_ui->ViewDateInput->date();
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

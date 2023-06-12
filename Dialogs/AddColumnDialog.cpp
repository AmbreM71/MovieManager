#include "AddColumnDialog.h"
#include "ui_AddColumnDialog.h"

AddColumnDialog::AddColumnDialog(QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::AddColumnDialog;
    m_ui->setupUi(this);

    m_ui->DoublePropertiesWidget->setVisible(false);
    m_ui->TextPropertiesWidget->setVisible(false);
    m_ui->IntPropertiesWidget->setVisible(false);

    // Adjust window size to fit the biggest case (double properties)
    m_ui->DoublePropertiesWidget->setVisible(true);
    this->adjustSize();
    m_ui->DoublePropertiesWidget->setVisible(false);
    m_ui->IntPropertiesWidget->setVisible(true);


    m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    QObject::connect(m_ui->TypeComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(TypeChanged(QString)));
    QObject::connect(m_ui->DoubleDecimals, SIGNAL(valueChanged(int)), this, SLOT(DecimalsPrecisionChanged(int)));
    QObject::connect(m_ui->NameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(IsColumnValid()));
    QObject::connect(m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(Validate()));
}

AddColumnDialog::~AddColumnDialog() {
    delete m_ui;
}

stColumn* AddColumnDialog::getColumn() {
    return &m_stColumn;
}

void AddColumnDialog::TypeChanged(QString sText) {
    m_ui->DoublePropertiesWidget->setVisible(false);
    m_ui->IntPropertiesWidget->setVisible(false);
    m_ui->TextPropertiesWidget->setVisible(false);
    if(QString::compare(sText, tr("Integer")) == 0) {
        m_ui->IntPropertiesWidget->setVisible(true);
    }
    else if(QString::compare(sText, tr("Text")) == 0) {
        m_ui->TextPropertiesWidget->setVisible(true);
    }
    else {
         m_ui->DoublePropertiesWidget->setVisible(true);
    }
}

void AddColumnDialog::DecimalsPrecisionChanged(int nPrecision) {
    m_ui->DoubleMinValue->setDecimals(nPrecision);
    m_ui->DoubleMaxValue->setDecimals(nPrecision);
}

void AddColumnDialog::IsColumnValid() {
    if(m_ui->NameLineEdit->text().length() > 0)
        m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    else
        m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void AddColumnDialog::Validate() {
    m_stColumn.sName = m_ui->NameLineEdit->text();
    if(QString::compare(m_ui->TypeComboBox->currentText(), tr("Integer")) == 0) {
        m_stColumn.eType = eColumnType::eColumnInteger;
        m_stColumn.nMin = m_ui->IntMinValue->value();
        m_stColumn.nMax = m_ui->IntMaxValue->value();
    }
    else if(QString::compare(m_ui->TypeComboBox->currentText(), tr("Text")) == 0) {
        m_stColumn.eType = eColumnType::eColumnText;
        m_stColumn.nMin = m_ui->IntMinValue->value();
        m_stColumn.nMax = m_ui->IntMaxValue->value();
        m_stColumn.nPrecision = m_ui->DoubleDecimals->value();
    }
    else {
        m_stColumn.eType = eColumnType::eColumnDouble;
        m_stColumn.textMaxLength = m_ui->TextMaxLength->value();
    }
}

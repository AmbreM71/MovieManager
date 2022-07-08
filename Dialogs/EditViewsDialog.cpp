#include "EditViewsDialog.h"
#include "ui_EditViewsDialog.h"

EditViewsDialog::EditViewsDialog(QTableWidget* table, Log* log, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::EditViewsDialog;
    m_ui->setupUi(this);
    m_MainWindowTable = table;
    m_log = log;

    QString name = m_MainWindowTable->item(m_MainWindowTable->currentRow(),0)->text();
    QString releaseYear = m_MainWindowTable->item(m_MainWindowTable->currentRow(),1)->text();
    QString entriesFR = m_MainWindowTable->item(m_MainWindowTable->currentRow(),5)->text();
    QString rating = m_MainWindowTable->item(m_MainWindowTable->currentRow(),6)->text();

    QSqlQuery query;
    query.prepare("SELECT Name, ViewDate, ViewType FROM movieViews WHERE Name='"+name+"' AND ReleaseYear='"+releaseYear+"' AND EntriesFR='"+entriesFR+"' AND Rating='"+rating+"' ORDER BY ViewDate DESC;");

    if(!query.exec()){
        m_log->append("Erreur lors de la récupération dans la base de données, plus d'informations ci-dessous :\nCode d'erreur "+query.lastError().nativeErrorCode()+" : "+query.lastError().text());
    }

    while(query.next()) {

         QTableWidgetItem* name = new QTableWidgetItem();
         QTableWidgetItem* viewDate = new QTableWidgetItem();
         QTableWidgetItem* viewType = new QTableWidgetItem();

         name->setText(query.value(0).toString());
         viewDate->setText(query.value(1).toString());
         viewType->setText(query.value(2).toString());

         m_ui->tableWidget->insertRow(m_ui->tableWidget->rowCount());

         m_ui->tableWidget->setItem(m_ui->tableWidget->rowCount()-1, 0, name);
         m_ui->tableWidget->setItem(m_ui->tableWidget->rowCount()-1, 1, viewDate);
         m_ui->tableWidget->setItem(m_ui->tableWidget->rowCount()-1, 2, viewType);
    }
}

EditViewsDialog::~EditViewsDialog() {
    delete m_ui;
}

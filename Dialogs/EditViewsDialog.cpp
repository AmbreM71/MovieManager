#include "EditViewsDialog.h"
#include "ui_EditViewsDialog.h"

EditViewsDialog::EditViewsDialog(QTableWidget* table, Log* log, int* theme, QWidget *parent) : QDialog(parent) {
    m_ui = new Ui::EditViewsDialog;
    m_ui->setupUi(this);
    m_MainWindowTable = table;
    m_log = log;
    m_theme = theme;

    QObject::connect(m_ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));

    fillTable();

}

EditViewsDialog::~EditViewsDialog() {
    delete m_ui;
}

void EditViewsDialog::fillTable() {

    int movieListRowCount = m_ui->tableWidget->rowCount();
    for(int i=movieListRowCount ; i >= 0 ; i--) {
        m_ui->tableWidget->removeRow(i);
    }

    QString name = m_MainWindowTable->item(m_MainWindowTable->currentRow(),0)->text();
    QString releaseYear = m_MainWindowTable->item(m_MainWindowTable->currentRow(),1)->text();
    QString entriesFR = m_MainWindowTable->item(m_MainWindowTable->currentRow(),5)->text();
    QString rating = m_MainWindowTable->item(m_MainWindowTable->currentRow(),6)->text();

    QSqlQuery query;
    query.prepare("SELECT Name, ViewDate, ViewType FROM movieViews WHERE Name=\""+name+"\" AND ReleaseYear="+releaseYear+" AND Entries="+entriesFR+" AND Rating="+rating+" ORDER BY ViewDate DESC;");

    if(!query.exec()){
        m_log->append(tr("Erreur lors de la récupération dans la base de données, plus d'informations ci-dessous :\nCode d'erreur ")+query.lastError().nativeErrorCode()+tr(" : ")+query.lastError().text());
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


void EditViewsDialog::customMenuRequested(QPoint pos) {
    QMenu *menu = new QMenu(this);
    QAction* deleteAction = new QAction(tr("Supprimer"), this);
    if(*m_theme == Theme::Classic) {
        deleteAction->setIcon(QIcon(":/icons/Icons/remove.png"));
    }
    else {
        deleteAction->setIcon(QIcon(":/icons/Icons/remove light.png"));
    }
    menu->addAction(deleteAction);
    QObject::connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteView()));
    menu->popup(m_ui->tableWidget->viewport()->mapToGlobal(pos));
}

bool EditViewsDialog::edited() {
    return m_edited;
}

void EditViewsDialog::deleteView() {
    QSqlQuery deleteQuery;

    QString name = m_MainWindowTable->item(m_MainWindowTable->currentRow(),0)->text();
    QString releaseYear = m_MainWindowTable->item(m_MainWindowTable->currentRow(),1)->text();
    QString entriesFR = m_MainWindowTable->item(m_MainWindowTable->currentRow(),5)->text();
    QString rating = m_MainWindowTable->item(m_MainWindowTable->currentRow(),6)->text();
    QString viewDate = m_ui->tableWidget->item(m_ui->tableWidget->currentRow(),1)->text();
    QString viewType = m_ui->tableWidget->item(m_ui->tableWidget->currentRow(),2)->text();

    deleteQuery.exec("DELETE FROM movieViews WHERE Name=\""+name+"\" AND ReleaseYear=\""+releaseYear+"\" AND Entries=\""+entriesFR+"\" AND Rating=\""+rating+"\" AND ViewDate=\""+viewDate+"\" AND ViewType=\""+viewType+"\";");
    fillTable();
    m_edited = true;
}

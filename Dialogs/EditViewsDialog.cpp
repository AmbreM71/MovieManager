#include "EditViewsDialog.h"
#include "ui_EditViewsDialog.h"

EditViewsDialog::EditViewsDialog(int* ID, Log* log, enum eTheme* theme, QString* dateFormat, QWidget* parent) : QDialog(parent) {
    m_ui = new Ui::EditViewsDialog;
    m_ui->setupUi(this);
    m_ID = ID;
    m_log = log;
    m_theme = theme;
    m_DateFormat = dateFormat;
    this->setWindowIcon(QIcon(":/assets/Assets/Icons/Dark/edit.png"));

    m_ui->tableWidget->setColumnHidden(0, true);

    QSqlQuery titleQuery;
    titleQuery.exec("SELECT Name FROM movies WHERE ID="+QString::number(*m_ID)+";");
    titleQuery.first();
    this->setWindowTitle(tr("Vues - ")+titleQuery.value(0).toString());

    QObject::connect(m_ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));
    QObject::connect(m_ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editView()));

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

    QSqlQuery query;
    query.prepare("SELECT ID, ViewDate, ViewType FROM views WHERE ID_Movie="+QString::number(*m_ID)+" ORDER BY ViewDate DESC;");

    if(!query.exec()){
        m_log->append(tr("Erreur lors de la récupération dans la base de données, plus d'informations ci-dessous :\nCode d'erreur ")+query.lastError().nativeErrorCode()+tr(" : ")+query.lastError().text(), eLog::Error);
    }

    while(query.next()) {

         QTableWidgetItem* ID = new QTableWidgetItem();
         QTableWidgetItem* viewDate = new QTableWidgetItem();
         QTableWidgetItem* viewType = new QTableWidgetItem();

         ID->setText(query.value(0).toString());
         if(query.value(1).toString() == "?")
             viewDate->setText("?");
         else
            viewDate->setText(query.value(1).toDate().toString(*m_DateFormat));
         viewType->setText(Common::viewTypeToQString((enum eViewType)query.value(2).toInt()));

         m_ui->tableWidget->insertRow(m_ui->tableWidget->rowCount());

         m_ui->tableWidget->setItem(m_ui->tableWidget->rowCount()-1, 0, ID);
         m_ui->tableWidget->setItem(m_ui->tableWidget->rowCount()-1, 1, viewDate);
         m_ui->tableWidget->setItem(m_ui->tableWidget->rowCount()-1, 2, viewType);
    }
}


void EditViewsDialog::customMenuRequested(QPoint pos) {
    QMenu *menu = new QMenu(this);

    QAction* editAction = new QAction(tr("Modifier"), this);
    Common::setIconAccordingToTheme(editAction, *m_theme, "edit");

    QAction* deleteAction = new QAction(tr("Supprimer"), this);
    Common::setIconAccordingToTheme(deleteAction, *m_theme, "delete.png");

    menu->addAction(editAction);
    menu->addAction(deleteAction);

    QObject::connect(editAction, SIGNAL(triggered()), this, SLOT(editView()));
    QObject::connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteView()));

    menu->popup(m_ui->tableWidget->viewport()->mapToGlobal(pos));
}

bool EditViewsDialog::edited() {
    return m_edited;
}

void EditViewsDialog::deleteView() {
    QSqlQuery deleteQuery;
    QString viewID = m_ui->tableWidget->item(m_ui->tableWidget->currentRow(),0)->text();

    deleteQuery.exec("DELETE FROM views WHERE ID=\""+viewID+"\";");
    fillTable();
    m_edited = true;
}

void EditViewsDialog::editView() {

    QString viewDate;
    int viewType;
    QString viewID = m_ui->tableWidget->item(m_ui->tableWidget->currentRow(),0)->text();

    EditViewDialog* window = new EditViewDialog(m_ui->tableWidget, m_DateFormat, this);
    window->show();
    if(window->exec() == 1) {
        QSqlQuery editMovieQuery;

        viewType = window->getViewType();
        viewDate = window->getViewDate().toString("yyyy-MM-dd");

        if(window->isDateUnknown()) {
            viewDate = "?";
        }
        if(window->isTypeUnknown()) {
            viewType = eViewType::Unknown;
        }
        editMovieQuery.exec("UPDATE views SET ViewDate=\""+viewDate+"\", ViewType=\""+QString::number(viewType)+"\" WHERE ID=\""+viewID+"\";");

        fillTable();
        m_edited = true;
    }
}

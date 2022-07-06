#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QApplication* app, QWidget *parent) {
    m_ui = new Ui::MainWindow;
    m_ui->setupUi(this);
    databaseConnection();
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::databaseConnection() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("movieDatabase.db");

    if(!m_db.open()) {
        qDebug() << "Erreur lors de l'ouverture de la base de données";
    }

    QString databaseCreationString = "CREATE TABLE movieViews ("
                                   "ID          MEDIUMINT,"
                                   "Name        VARCHAR(127),"
                                   "ReleaseYear SMALLINT,"
                                   "viewDate    DATE"
                                   "viewType    VARCHAR(255));";

    QSqlQuery movieViewsQuery;

    if(!movieViewsQuery.exec(databaseCreationString))
        qDebug()<<"Error creating table contact or table already existing";


}

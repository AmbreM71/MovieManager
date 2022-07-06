#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Dialogs/AddViewDialog.h"

MainWindow::MainWindow(QApplication* app, QWidget *parent) {
    m_ui = new Ui::MainWindow;
    m_ui->setupUi(this);
    databaseConnection();

    QObject::connect(m_ui->AddViewButton, SIGNAL(clicked()), this, SLOT(addView()));
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
    //Grade and EntriesFR shouldn't be filled when user add a view, entriesFR should get value from jpbox-office.com and grade only if the movie isn't already in the list
    QString databaseCreationString = "CREATE TABLE movieViews ("
                                   "ID          MEDIUMINT,"
                                   "Name        VARCHAR(127),"
                                   "ReleaseYear SMALLINT,"
                                   "ViewDate    DATE,"
                                   "EntriesFR   INT,"
                                   "Grade       TINYINT(10),"
                                   "ViewType    VARCHAR(63));";

    QSqlQuery movieViewsQuery;

    if(!movieViewsQuery.exec(databaseCreationString))
        qDebug()<<"Error creating table contact or table already existing";


}

void MainWindow::addView() {
    AddViewDialog* window = new AddViewDialog();
    window->show();
}

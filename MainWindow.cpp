#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Dialogs/AddViewDialog.h"

MainWindow::MainWindow(QApplication* app, QWidget *parent) {
    m_ui = new Ui::MainWindow;
    m_ui->setupUi(this);
    databaseConnection();
    loadDB();

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
                                   "Name        VARCHAR(127),"
                                   "ReleaseYear SMALLINT,"
                                   "ViewDate    DATE,"
                                   "EntriesFR   INT,"
                                   "Rating       TINYINT(10),"
                                   "ViewType    VARCHAR(63));";

    QSqlQuery movieViewsQuery;

    if(!movieViewsQuery.exec(databaseCreationString))
        qDebug()<<"Error creating table or table already existing";


}

void MainWindow::loadDB() {
    //Fetch every unique movies
    QSqlQuery moviesQuery;
    moviesQuery.exec("SELECT Name, ReleaseYear, EntriesFR, Rating FROM movieViews GROUP BY Name, ReleaseYear, EntriesFR, Rating;");

    while(moviesQuery.next()) {
        QTableWidgetItem* name = new QTableWidgetItem();
        QTableWidgetItem* releaseYear = new QTableWidgetItem();
        QTableWidgetItem* numberOfViews = new QTableWidgetItem();
        QTableWidgetItem* firstSeen = new QTableWidgetItem();
        QTableWidgetItem* lastSeen = new QTableWidgetItem();
        QTableWidgetItem* entriesFR = new QTableWidgetItem();
        QTableWidgetItem* rating = new QTableWidgetItem();

        //Fetch the number of views of the current movie
        QSqlQuery viewsQuery;
        viewsQuery.exec("SELECT COUNT(*) FROM movieViews WHERE Name='"+moviesQuery.value(0).toString()+"' AND ReleaseYear='"+moviesQuery.value(1).toString()+"' AND EntriesFR='"+moviesQuery.value(2).toString()+"' AND Rating='"+moviesQuery.value(3).toString()+"'");
        viewsQuery.first();
        numberOfViews->setText(viewsQuery.value(0).toString());

        //Fetch the first view of the current movie
        QSqlQuery firstViewQuery;
        firstViewQuery.exec("SELECT ViewDate FROM movieViews WHERE Name='"+moviesQuery.value(0).toString()+"' AND ReleaseYear='"+moviesQuery.value(1).toString()+"' AND EntriesFR='"+moviesQuery.value(2).toString()+"' AND Rating='"+moviesQuery.value(3).toString()+"' ORDER BY ViewDate ASC LIMIT 1");
        firstViewQuery.first();
        firstSeen->setText(firstViewQuery.value(0).toString());

        //Fetch the last view of the current movie
        QSqlQuery lastViewQuery;
        lastViewQuery.exec("SELECT ViewDate FROM movieViews WHERE Name='"+moviesQuery.value(0).toString()+"' AND ReleaseYear='"+moviesQuery.value(1).toString()+"' AND EntriesFR='"+moviesQuery.value(2).toString()+"' AND Rating='"+moviesQuery.value(3).toString()+"' ORDER BY ViewDate DESC LIMIT 1");
        lastViewQuery.first();
        lastSeen->setText(lastViewQuery.value(0).toString());

        name->setText(moviesQuery.value(0).toString());
        releaseYear->setText(moviesQuery.value(1).toString());
        entriesFR->setText(moviesQuery.value(2).toString());
        rating->setText(moviesQuery.value(3).toString());

        //Creates a new row on the table
        m_ui->MoviesListWidget->insertRow(m_ui->MoviesListWidget->rowCount());

        //Insert QTableWidgetItem in the table
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 0, name);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 1, releaseYear);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 2, numberOfViews);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 3, firstSeen);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 4, lastSeen);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 5, entriesFR);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 6, rating);
    }
}

void MainWindow::addView() {
    AddViewDialog* window = new AddViewDialog();
    window->show();
    if(window->exec() == 1) {
        QSqlQuery query;
        query.prepare("INSERT INTO movieViews (Name, ReleaseYear, ViewDate, ViewType, Rating) VALUES (?,?,?,?,?);");
        query.addBindValue(window->getName());
        query.addBindValue(window->getReleaseYear());
        query.addBindValue(window->getViewDate());
        query.addBindValue(window->getViewType());
        query.addBindValue(window->getRating());

        if(!query.exec()){
            qDebug() << "Error while adding a view to the database, see more below :";
            qDebug() << query.lastError().text();
        }
    }
}

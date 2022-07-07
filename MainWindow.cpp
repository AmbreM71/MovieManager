#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Dialogs/AddViewDialog.h"

MainWindow::MainWindow(QApplication* app, QWidget *parent) {
    m_ui = new Ui::MainWindow;
    m_ui->setupUi(this);
    databaseConnection();
    loadDB();
    fillGlobalStats();
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
        query.prepare("INSERT INTO movieViews (Name, ReleaseYear, ViewDate, ViewType, Rating, EntriesFR) VALUES (?,?,?,?,?,?);");

        //If nothing is selected in the combobox
        if(window->getComboboxSelectedItem() == "") {
            query.bindValue(0, window->getName());
            query.bindValue(1, window->getReleaseYear());
            query.bindValue(4, window->getRating());
            query.bindValue(5, 0);
        }
        else {
            QString movieName = window->getComboboxSelectedItem().remove(window->getComboboxSelectedItem().length()-7, window->getComboboxSelectedItem().length());
            QString movieYear = window->getComboboxSelectedItem().remove(0, window->getComboboxSelectedItem().length()-4);

            QSqlQuery ratingQuery;
            ratingQuery.exec("SELECT Rating, EntriesFR FROM movieViews WHERE Name='"+movieName+"' AND ReleaseYear='"+movieYear+"' GROUP BY Rating");
            ratingQuery.first();

            query.bindValue(0, movieName);
            query.bindValue(1, movieYear);
            query.bindValue(4, ratingQuery.value(0).toString());
            query.bindValue(5, ratingQuery.value(1).toInt());
        }

        query.bindValue(2, window->getViewDate());
        query.bindValue(3, window->getViewType());

        if(!query.exec()){
            qDebug() << "Error while adding a view to the database, see more below :";
            qDebug() << query.lastError().text();
        }
    }
}

void MainWindow::fillGlobalStats() {

    QSqlQuery totalViewQuery;
    totalViewQuery.exec("SELECT COUNT(*) FROM movieViews;");
    totalViewQuery.first();

    QSqlQuery avgMovieYearQuery;
    avgMovieYearQuery.exec("SELECT ReleaseYear FROM movieViews GROUP BY Name, ReleaseYear, EntriesFR, Rating;");
    float avgMovieYear = 0;
    float i=0;
    while(avgMovieYearQuery.next()) {
        avgMovieYear += (float)avgMovieYearQuery.value(0).toInt();
        i++;
    }
    avgMovieYear /= i;

    float avgViews = totalViewQuery.value(0).toFloat()/(float)m_ui->MoviesListWidget->rowCount();
    avgViews = round(avgViews*100)/100;

    QSqlQuery movieThisYearQuery;
    int movieThisYear=0;
    movieThisYearQuery.exec("SELECT * FROM movieViews WHERE ViewDate BETWEEN '2022-01-01' AND '2022-12-31' GROUP BY Name, ReleaseYear, EntriesFR, Rating;");
    while(movieThisYearQuery.next()) {
        movieThisYear++;
    }


    m_ui->TotalMoviesLabel->setText("Nombre de films vus : " + QString::number(m_ui->MoviesListWidget->rowCount()));
    m_ui->TotalViewLabel->setText("Nombre total de visionnages : " + totalViewQuery.value(0).toString());
    m_ui->AverageViewLabel->setText("Moyenne de visionnages : " + QString::number(avgViews));
    m_ui->AverageYearLabel->setText("Année moyenne des films vus : " + QString::number(avgMovieYear));
    m_ui->ViewThisYear->setText("Films vus cette année : " + QString::number(movieThisYear));
}

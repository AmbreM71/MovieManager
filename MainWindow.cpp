#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QApplication* app, QWidget* parent) {

    m_app = app;
    m_ui = new Ui::MainWindow;
    m_log = new Log();
    m_ui->setupUi(this);

    setSettings();
    refreshLanguage();

    m_ui->MoviesListWidget->setHorizontalHeaderLabels(QStringList() << tr("Nom du film") << tr("Année\nde sortie") << tr("Nombre de\nvisionnages") << tr("Premier\nvisionnage") << tr("Dernier\nvisionnage") << tr("Entrées") << tr("Note"));

    databaseConnection();
    loadDB();
    fillGlobalStats();
    menuBarConnectors();

    m_ui->MoviesListWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_ui->MoviesListWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    m_ui->MoviesListWidget->setColumnWidth(1,100);
    m_ui->MoviesListWidget->setColumnWidth(2,100);
    m_ui->MoviesListWidget->setColumnWidth(3,100);
    m_ui->MoviesListWidget->setColumnWidth(4,100);
    m_ui->MoviesListWidget->setColumnWidth(5,100);
    m_ui->MoviesListWidget->setColumnWidth(6,70);


    QObject::connect(m_ui->AddViewButton, SIGNAL(clicked()), this, SLOT(addView()));
    QObject::connect(m_ui->ManageMovieViewsButton, SIGNAL(clicked()), this, SLOT(editViews()));
    QObject::connect(m_ui->AdvancedSearchButton, SIGNAL(clicked()), this, SLOT(openFilters()));
    QObject::connect(m_ui->ResetFiltersButton, SIGNAL(clicked()), this, SLOT(resetFilters()));
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::databaseConnection() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("movieDatabase.db");

    if(!m_db.open()) {
        m_log->append(tr("Erreur lors de l'ouverture de la base de données"));
    }
    //Grade and Entries shouldn't be filled when user add a view, Entries should get value from jpbox-office.com and grade only if the movie isn't already in the list
    QString databaseCreationString = "CREATE TABLE movieViews ("
                                   "Name        VARCHAR(127),"
                                   "ReleaseYear SMALLINT,"
                                   "ViewDate    DATE,"
                                   "Entries   INT,"
                                   "Rating          TINYINT(10),"
                                   "ViewType    VARCHAR(63));";

    QSqlQuery movieViewsQuery;

    if(!movieViewsQuery.exec(databaseCreationString)) {
        m_log->append(tr("Erreur lors de la création de la base de données, elle existe peut-être déjà"));
    }

}

void MainWindow::loadDB(bool isFiltered) {

    m_ui->MoviesListWidget->sortItems(0);
    //Clear the table
    int movieListRowCount = m_ui->MoviesListWidget->rowCount();
    for(int i=movieListRowCount ; i >= 0 ; i--) {
        m_ui->MoviesListWidget->removeRow(i);
    }


    //Fetch every unique movies
    QSqlQuery moviesQuery;

    if(isFiltered) {

        moviesQuery.exec("SELECT Name, ReleaseYear, Entries, Rating FROM movieViews "
                         "WHERE Name LIKE '%" + m_filter_name + "%'"
                         "AND ReleaseYear BETWEEN '"+QString::number(m_filter_minYear)+"' AND '"+QString::number(m_filter_maxYear)+"'"
                         "AND Rating BETWEEN '"+QString::number(m_filter_minRating)+"' AND '"+QString::number(m_filter_maxRating)+"'"
                         "AND Entries >= "+QString::number(m_filter_minEntries)+" "
                         "GROUP BY Name, ReleaseYear, Entries, Rating;");
        m_ui->ResetFiltersButton->setEnabled(true);
    }
    else {
        moviesQuery.exec("SELECT Name, ReleaseYear, Entries, Rating FROM movieViews GROUP BY Name, ReleaseYear, Entries, Rating;");
        m_ui->ResetFiltersButton->setEnabled(false);
        m_filter_name = "";
        m_filter_minYear = 0;
        m_filter_maxYear = 0;
        m_filter_minRating = 0;
        m_filter_maxRating = 0;
        m_filter_minEntries = 0;
    }

    while(moviesQuery.next()) {
        QTableWidgetItem* name = new QTableWidgetItem();
        QTableWidgetItem* releaseYear = new QTableWidgetItem();
        QTableWidgetItem* numberOfViews = new QTableWidgetItem();
        QTableWidgetItem* firstSeen = new QTableWidgetItem();
        QTableWidgetItem* lastSeen = new QTableWidgetItem();
        QTableWidgetItem* Entries = new QTableWidgetItem();
        QTableWidgetItem* rating = new QTableWidgetItem();

        releaseYear->setTextAlignment(Qt::AlignCenter);
        firstSeen->setTextAlignment(Qt::AlignCenter);
        lastSeen->setTextAlignment(Qt::AlignCenter);


        //Fetch the number of views of the current movie
        QSqlQuery viewsQuery;
        viewsQuery.exec("SELECT COUNT(*) FROM movieViews WHERE Name='"+moviesQuery.value(0).toString()+"' AND ReleaseYear='"+moviesQuery.value(1).toString()+"' AND Entries='"+moviesQuery.value(2).toString()+"' AND Rating='"+moviesQuery.value(3).toString()+"'");
        viewsQuery.first();
        numberOfViews->setText(viewsQuery.value(0).toString());

        //Fetch the first view of the current movie
        QSqlQuery firstViewQuery;
        firstViewQuery.exec("SELECT ViewDate FROM movieViews WHERE Name='"+moviesQuery.value(0).toString()+"' AND ReleaseYear='"+moviesQuery.value(1).toString()+"' AND Entries='"+moviesQuery.value(2).toString()+"' AND Rating='"+moviesQuery.value(3).toString()+"' AND NOT ViewDate='?' ORDER BY ViewDate ASC LIMIT 1");
        firstViewQuery.first();
        firstSeen->setText(firstViewQuery.value(0).toString());

        //Fetch the last view of the current movie
        QSqlQuery lastViewQuery;
        lastViewQuery.exec("SELECT ViewDate FROM movieViews WHERE Name='"+moviesQuery.value(0).toString()+"' AND ReleaseYear='"+moviesQuery.value(1).toString()+"' AND Entries='"+moviesQuery.value(2).toString()+"' AND Rating='"+moviesQuery.value(3).toString()+"' AND NOT ViewDate='?' ORDER BY ViewDate DESC LIMIT 1");
        lastViewQuery.first();
        lastSeen->setText(lastViewQuery.value(0).toString());

        QSqlQuery hasUnknownView;
        hasUnknownView.exec("SELECT ViewDate FROM movieViews WHERE Name='"+moviesQuery.value(0).toString()+"' AND ReleaseYear='"+moviesQuery.value(1).toString()+"' AND Entries='"+moviesQuery.value(2).toString()+"' AND Rating='"+moviesQuery.value(3).toString()+"' AND ViewDate='?'");
        hasUnknownView.first();
        if(!hasUnknownView.isNull(0)) {
            firstSeen->setForeground(QBrush(QColor(255,0,0)));
        }

        name->setText(moviesQuery.value(0).toString());
        releaseYear->setText(moviesQuery.value(1).toString());
        Entries->setText(moviesQuery.value(2).toString());
        rating->setText(moviesQuery.value(3).toString());

        if((name->text() == "Matrix" || name->text() == "The Matrix") && m_matrixMode) {
            name->setForeground(QBrush(QColor(0,150,0)));
        }

        //Creates a new row on the table
        m_ui->MoviesListWidget->insertRow(m_ui->MoviesListWidget->rowCount());

        //Insert QTableWidgetItem in the table
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 0, name);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 1, releaseYear);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 2, numberOfViews);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 3, firstSeen);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 4, lastSeen);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 5, Entries);
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 6, rating);
    }
    m_ui->MoviesListWidget->setCurrentCell(0,0);
}

void MainWindow::addView() {
    AddViewDialog* window = new AddViewDialog();
    window->show();
    if(window->exec() == 1) {

        QSqlQuery query;
        query.prepare("INSERT INTO movieViews (Name, ReleaseYear, ViewDate, ViewType, Rating, Entries) VALUES (?,?,?,?,?,?);");

        //If nothing is selected in the combobox
        if(window->getComboboxSelectedItem() == "") {
            query.bindValue(0, window->getName());
            query.bindValue(1, window->getReleaseYear());
            query.bindValue(4, window->getRating());
            query.bindValue(5, window->getEntries());
        }
        else {
            QString movieName = window->getComboboxSelectedItem().remove(window->getComboboxSelectedItem().length()-7, window->getComboboxSelectedItem().length());
            QString movieYear = window->getComboboxSelectedItem().remove(0, window->getComboboxSelectedItem().length()-4);

            QSqlQuery ratingQuery;
            ratingQuery.exec("SELECT Rating, Entries FROM movieViews WHERE Name='"+movieName+"' AND ReleaseYear='"+movieYear+"' GROUP BY Rating");
            ratingQuery.first();

            query.bindValue(0, movieName);
            query.bindValue(1, movieYear);
            query.bindValue(4, ratingQuery.value(0).toString());
            query.bindValue(5, ratingQuery.value(1).toInt());
        }

        if(window->isDateUnknown()) {
            query.bindValue(2, "?");
            query.bindValue(3, "?");
        }
        else {
            query.bindValue(2, window->getViewDate());
            query.bindValue(3, window->getViewType());
        }

        if(!query.exec()){
            m_log->append(tr("Erreur lors de l'ajout dans la base de données, plus d'informations ci-dessous :\nCode d'erreur ")+query.lastError().nativeErrorCode()+tr(" : ")+query.lastError().text());
        }
        fillGlobalStats();
        loadDB();
    }
}

void MainWindow::editViews() {
    EditViewsDialog* window = new EditViewsDialog(m_ui->MoviesListWidget, m_log);
    window->show();
    if(window->exec() == 1) {
        if (window->edited()) {
            loadDB();
            fillGlobalStats();
        }
    }
}

void MainWindow::openFilters() {
    FiltersDialog* window = new FiltersDialog(&m_filter_name, &m_filter_minYear, &m_filter_maxYear, &m_filter_minRating, &m_filter_maxRating, &m_filter_minEntries);
    window->show();
    if(window->exec() == 1) {
        delete window;
        loadDB(true);
    }
}

void MainWindow::openLog() {
    if(LogDialog::instancesCount() == 0) {
        LogDialog* window = new LogDialog(m_log, this);
        window->show();
        if(window->exec() == 0) {
            delete window;
        }
    }
}

void MainWindow::openAbout() {
    if(AboutDialog::instancesCount() == 0) {
        AboutDialog* window = new AboutDialog(this);
        window->show();
        if(window->exec() == 0) {
            delete window;
        }
    }
}

void MainWindow::openSettings() {
    OptionsDialog* window = new OptionsDialog(&m_matrixMode, &m_language, &m_theme);
    window->show();
    if(window->exec() == 1) {
        delete window;
        refreshLanguage();
        loadDB();
    }
}

void MainWindow::resetFilters() {
    m_ui->ResetFiltersButton->setEnabled(false);
    m_filter_name = "";
    m_filter_minYear = 0;
    m_filter_maxYear = 0;
    m_filter_minRating = 0;
    m_filter_maxRating = 0;
    m_filter_minEntries = 0;
    loadDB();
}

void MainWindow::menuBarConnectors() {
    QObject::connect(m_ui->actionQuitter, SIGNAL(triggered()), m_app, SLOT(quit()));
    QObject::connect(m_ui->actionLog, SIGNAL(triggered()), this, SLOT(openLog()));
    QObject::connect(m_ui->action_Propos, SIGNAL(triggered()), this, SLOT(openAbout()));
    QObject::connect(m_ui->actionPr_f_rences, SIGNAL(triggered()), this, SLOT(openSettings()));

}

void MainWindow::setSettings() {
    m_matrixMode = false;
    m_theme = Theme::MidnightPurple;
    m_language = Language::English;
}

void MainWindow::refreshLanguage() {
    bool successLoad = false;
    QString path;

    switch(m_language) {
        case Language::English :
            path = ":/localisations/Localisation/MovieManager_en_US.qm";
            break;
        case Language::French :
            path = ":/localisations/Localisation/MovieManager_fr_FR.qm";
            break;
    }

    if(m_translator.load(path)) {
        m_app->installTranslator(&m_translator);
        successLoad = true;
    }

    if(!successLoad) {
        m_log->append(tr("Impossible de charger le fichier de langage"));
    }
    m_ui->retranslateUi(this);
}

void MainWindow::fillGlobalStats() {

    QSqlQuery totalViewQuery;
    totalViewQuery.exec("SELECT COUNT(*) FROM movieViews;");
    totalViewQuery.first();

    QSqlQuery avgMovieYearQuery;
    avgMovieYearQuery.exec("SELECT ReleaseYear, Rating FROM movieViews GROUP BY Name, ReleaseYear, Entries, Rating;");
    float avgMovieYear = 0;
    float avgRating = 0;
    float i=0;
    while(avgMovieYearQuery.next()) {
        avgMovieYear += (float)avgMovieYearQuery.value(0).toInt();
        avgRating += (float)avgMovieYearQuery.value(1).toInt();
        i++;
    }

    avgMovieYear /= i;
    avgRating /= i;



    float avgViews = totalViewQuery.value(0).toFloat()/(float)m_ui->MoviesListWidget->rowCount();
    avgViews = round(avgViews*100)/100;
    avgRating = round(avgRating*100)/100;

    QSqlQuery movieThisYearQuery;
    int movieThisYear=0;
    movieThisYearQuery.exec("SELECT * FROM movieViews WHERE ViewDate BETWEEN '"+QString::number(QDate::currentDate().year())+"-01-01' AND '"+QString::number(QDate::currentDate().year())+"-12-31' GROUP BY Name, ReleaseYear, Entries, Rating;");
    while(movieThisYearQuery.next()) {
        movieThisYear++;
    }



    m_ui->TotalMoviesLabel->setText(tr("Nombre de films vus : ") + QString::number(m_ui->MoviesListWidget->rowCount()));
    m_ui->TotalViewLabel->setText(tr("Nombre total de visionnages : ") + totalViewQuery.value(0).toString());
    m_ui->AverageViewLabel->setText(tr("Moyenne de visionnages : ") + QString::number(avgViews));
    m_ui->AverageYearLabel->setText(tr("Année moyenne des films vus : ") + QString::number(avgMovieYear));
    m_ui->AverageRatingLabel->setText(tr("Note moyenne : ") + QString::number(avgRating));
    m_ui->ViewThisYear->setText(tr("Films vus cette année : ") + QString::number(movieThisYear));
}

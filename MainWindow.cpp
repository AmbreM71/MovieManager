#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <ShlObj.h>


MainWindow::MainWindow(QApplication* app, QWidget* parent) {

    m_app = app;
    m_ui = new Ui::MainWindow;
    m_log = new Log();
    m_settings = new QSettings("MovieManager", "MovieManager");
    m_ui->setupUi(this);

    m_app->setWindowIcon(QIcon(":/icons/Icons/logo.png"));

    setSettings();
    refreshLanguage();
    refreshTheme();

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
    m_ui->MoviesListWidget->setColumnHidden(7, true);

    QObject::connect(m_ui->AddViewButton, SIGNAL(clicked()), this, SLOT(addView()));
    QObject::connect(m_ui->ManageMovieViewsButton, SIGNAL(clicked()), this, SLOT(editViews()));
    QObject::connect(m_ui->MoviesListWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editViews()));
    QObject::connect(m_ui->AdvancedSearchButton, SIGNAL(clicked()), this, SLOT(openFilters()));
    QObject::connect(m_ui->ResetFiltersButton, SIGNAL(clicked()), this, SLOT(resetFilters()));
    QObject::connect(m_ui->MoviesListWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customMenuRequested(QPoint)));
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

    QString movieDatabaseCreationString = "CREATE TABLE movies ("
                                   "ID          INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "Name        VARCHAR(127),"
                                   "ReleaseYear SMALLINT,"
                                   "Entries   INT,"
                                   "Rating          TINYINT(10));";

    QSqlQuery movieDBQuery;

    if(!movieDBQuery.exec(movieDatabaseCreationString)) {
        m_log->append(tr("Erreur lors de la création de la table movies, elle existe peut-être déjà"));
    }


    QString ViewsDatabaseCreationString = "CREATE TABLE views ("
                                   "ID          INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "ID_Movie    INTEGER,"
                                   "ViewDate    DATE,"
                                   "ViewType    VARCHAR(63));";

    QSqlQuery viewsBDQuery;

    if(!viewsBDQuery.exec(ViewsDatabaseCreationString)) {
        m_log->append(tr("Erreur lors de la création de la table views, elle existe peut-être déjà"));
    }


}

void MainWindow::loadDB(bool isFiltered) {

    //Clear the table
    int movieListRowCount = m_ui->MoviesListWidget->rowCount();
    for(int i=movieListRowCount ; i >= 0 ; i--) {
        m_ui->MoviesListWidget->removeRow(i);
    }


    //Fetch every unique movies
    QSqlQuery moviesQuery;

    if(isFiltered) {

        moviesQuery.exec("SELECT ID, Name, ReleaseYear, Entries, Rating FROM movies "
                         "WHERE Name LIKE \"%" + m_filter_name + "%\""
                         "AND ReleaseYear BETWEEN '"+QString::number(m_filter_minYear)+"' AND '"+QString::number(m_filter_maxYear)+"'"
                         "AND Rating BETWEEN '"+QString::number(m_filter_minRating)+"' AND '"+QString::number(m_filter_maxRating)+"'"
                         "AND Entries >= "+QString::number(m_filter_minEntries)+" "
                         "GROUP BY Name, ReleaseYear, Entries, Rating;");
        m_ui->ResetFiltersButton->setEnabled(true);
    }
    else {
        moviesQuery.exec("SELECT ID, Name, ReleaseYear, Entries, Rating FROM movies;");
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
        QTableWidgetItem* ID = new QTableWidgetItem();

        releaseYear->setTextAlignment(Qt::AlignCenter);
        firstSeen->setTextAlignment(Qt::AlignCenter);
        lastSeen->setTextAlignment(Qt::AlignCenter);

        //Fetch the number of views of the current movie
        QSqlQuery viewsQuery;
        viewsQuery.exec("SELECT COUNT(*) FROM views WHERE ID_Movie='"+moviesQuery.value(0).toString()+"'");
        viewsQuery.first();
        numberOfViews->setText(viewsQuery.value(0).toString());

        //Fetch the first view of the current movie
        QSqlQuery firstViewQuery;
        firstViewQuery.exec("SELECT ViewDate FROM views WHERE ID_Movie='"+moviesQuery.value(0).toString()+"' AND NOT ViewDate='?' ORDER BY ViewDate ASC LIMIT 1");
        firstViewQuery.first();
        if(firstViewQuery.value(0).toString()=="") {
            firstSeen->setText("?");
        }
        else {
            firstSeen->setText(firstViewQuery.value(0).toString());
        }

        //Fetch the last view of the current movie
        QSqlQuery lastViewQuery;
        lastViewQuery.exec("SELECT ViewDate FROM views WHERE ID_Movie='"+moviesQuery.value(0).toString()+"' AND NOT ViewDate='?' ORDER BY ViewDate DESC LIMIT 1");
        lastViewQuery.first();
        if(lastViewQuery.value(0).toString()=="") {
            lastSeen->setText("?");
        }
        else {
            lastSeen->setText(lastViewQuery.value(0).toString());
        }

        QSqlQuery hasUnknownView;
        hasUnknownView.exec("SELECT ViewDate FROM views WHERE ID_Movie='"+moviesQuery.value(0).toString()+"' AND ViewDate='?'");
        hasUnknownView.first();
        if(!hasUnknownView.isNull(0)) {
            if(firstSeen->text()!="?") {
                firstSeen->setForeground(QBrush(QColor(255,0,0)));
            }
        }
        ID->setText(moviesQuery.value(0).toString());
        name->setText(moviesQuery.value(1).toString());
        releaseYear->setText(moviesQuery.value(2).toString());
        Entries->setText(moviesQuery.value(3).toString());
        rating->setText(moviesQuery.value(4).toString());

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
        m_ui->MoviesListWidget->setItem(m_ui->MoviesListWidget->rowCount()-1, 7, ID);
    }
    m_ui->MoviesListWidget->setCurrentCell(0,0);

    //Disable Manage views and filters button if no movie the list is empty
    if(m_ui->MoviesListWidget->currentRow() == -1) {
        m_ui->ManageMovieViewsButton->setEnabled(false);
    }
    else {
        m_ui->ManageMovieViewsButton->setEnabled(true);
    }
}

void MainWindow::importDB() {
    QString file = QFileDialog::getOpenFileName(this, tr("Exporter"), QString(), "JSON (*.json)");
    QFile jsonFile(file);
    //Test if the file is correctly opened
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Erreur"), jsonFile.errorString());
    }
    else {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Importer"));
        msgBox.setText(tr("Souhaitez vous remplacer le contenu actuel ou ajouter au contenu actuel ?"));
        QAbstractButton* appendButton = msgBox.addButton(tr("Ajouter"), QMessageBox::YesRole);
        QAbstractButton* replaceButton = msgBox.addButton(tr("Remplacer"), QMessageBox::YesRole);
        msgBox.addButton(tr("Annuler"), QMessageBox::NoRole);
        msgBox.exec();
        if (msgBox.clickedButton()==replaceButton || msgBox.clickedButton()==appendButton) {
            bool add = true;
            if (msgBox.clickedButton()==replaceButton) {
                QMessageBox confirmReplaceBox;
                confirmReplaceBox.setText(tr("Cette opération va supprimer tous vos visionnages actuels, voulez-vous continuer ?"));
                QAbstractButton* yesButton = confirmReplaceBox.addButton(QMessageBox::Yes);
                confirmReplaceBox.addButton(QMessageBox::No);
                confirmReplaceBox.exec();
                if(confirmReplaceBox.clickedButton()==yesButton) {
                    QSqlQuery deleteQuery;
                    deleteQuery.exec("DELETE FROM movies");
                    deleteQuery.exec("DELETE FROM views");
                }
                else {
                    add = false;
                }
            }
            if(add) {
                QString val = jsonFile.readAll();
                jsonFile.close();
                QJsonObject main = QJsonDocument::fromJson(val.toUtf8()).object();
                foreach(const QString& mainKey, main.keys()) {
                    if(mainKey == "movies") {
                        QJsonObject movies = main.value(mainKey).toObject();
                        foreach(const QString& movieKey, movies.keys()) {
                            QJsonObject movie = movies.value(movieKey).toObject();
                            QSqlQuery query;
                            query.prepare("INSERT INTO movies (ID, Name, ReleaseYear, Entries, Rating) VALUES (?,?,?,?,?);");
                            query.bindValue(0, movie["ID"].toInt());
                            query.bindValue(1, movie["Name"].toString());
                            query.bindValue(2, movie["ReleaseYear"].toInt());
                            query.bindValue(3, movie["Entries"].toInt());
                            query.bindValue(4, movie["Rating"].toInt());

                            if(!query.exec()){
                                m_log->append(tr("Erreur lors de l'import dans la table movies, plus d'informations ci-dessous :\nCode d'erreur ")+query.lastError().nativeErrorCode()+tr(" : ")+query.lastError().text());
                            }
                        }
                    }
                    else if(mainKey == "views") {
                        QJsonObject views = main.value(mainKey).toObject();
                        foreach(const QString& viewKey, views.keys()) {
                            QJsonObject view = views.value(viewKey).toObject();
                            QSqlQuery query;
                            query.prepare("INSERT INTO views (ID, ID_Movie, ViewDate, ViewType) VALUES (?,?,?,?);");
                            query.bindValue(0, view["ID"].toInt());
                            query.bindValue(1, view["ID_Movie"].toInt());
                            query.bindValue(2, view["ViewDate"].toString());
                            query.bindValue(3, view["ViewType"].toString());

                            if(!query.exec()){
                                m_log->append(tr("Erreur lors de l'import dans la table views, plus d'informations ci-dessous :\nCode d'erreur ")+query.lastError().nativeErrorCode()+tr(" : ")+query.lastError().text());
                            }
                        }
                    }
                }
            }
        }
    }
    loadDB();
    fillGlobalStats();
}

void MainWindow::exportDB() {
    QString file = QFileDialog::getSaveFileName(this, tr("Exporter"), QString(), "JSON (*.json)");  //Get the save link
    //Creates a QFile with the fetched path
    QFile jsonFile(file);
    //Test if the file is correctly opened
    if (!jsonFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Erreur"), jsonFile.errorString());
    }

    QJsonObject mainObject;

    //Writes movies to JSON
    QJsonObject moviesObject;
    QSqlQuery moviesQuery;
    moviesQuery.exec("SELECT ID, Name, ReleaseYear, Entries, Rating FROM movies;");
    int i=0;
    while(moviesQuery.next()) {
        i++;

        QJsonObject movieObject;

        movieObject.insert("ID", QJsonValue::fromVariant(moviesQuery.value(0).toInt()));
        movieObject.insert("Name", QJsonValue::fromVariant(moviesQuery.value(1).toString()));
        movieObject.insert("ReleaseYear", QJsonValue::fromVariant(moviesQuery.value(2).toInt()));
        movieObject.insert("Entries", QJsonValue::fromVariant(moviesQuery.value(3).toInt()));
        movieObject.insert("Rating", QJsonValue::fromVariant(moviesQuery.value(4).toInt()));

        moviesObject.insert("movie" + QString::fromStdString(std::to_string(i)), movieObject);
    }
    mainObject.insert("movies", moviesObject);

    //Writes views to JSON
    QJsonObject viewsObject;
    QSqlQuery viewsQuery;
    viewsQuery.exec("SELECT ID, ID_Movie, ViewDate, ViewType FROM views;");
    int j=0;
    while(viewsQuery.next()) {
        j++;

        QJsonObject viewObject;

        viewObject.insert("ID", QJsonValue::fromVariant(viewsQuery.value(0).toInt()));
        viewObject.insert("ID_Movie", QJsonValue::fromVariant(viewsQuery.value(1).toInt()));
        viewObject.insert("ViewDate", QJsonValue::fromVariant(viewsQuery.value(2).toString()));
        viewObject.insert("ViewType", QJsonValue::fromVariant(viewsQuery.value(3).toString()));

        viewsObject.insert("view" + QString::fromStdString(std::to_string(j)), viewObject);
    }
    mainObject.insert("views", viewsObject);

    jsonFile.write(QJsonDocument(mainObject).toJson(QJsonDocument::Indented));

    jsonFile.close();
}

void MainWindow::addView() {
    AddViewDialog* window = new AddViewDialog();
    window->show();
    if(window->exec() == 1) {

        //Add the new movie to the movies table
        QSqlQuery insertIntoMoviesQuery;
        insertIntoMoviesQuery.prepare("INSERT INTO movies (Name, ReleaseYear, Entries, Rating) VALUES (?,?,?,?);");

        QString movieName;
        QString movieYear;

        if(window->getComboboxSelectedItem() == "") {
            insertIntoMoviesQuery.bindValue(0, window->getName());
            insertIntoMoviesQuery.bindValue(1, window->getReleaseYear());
            insertIntoMoviesQuery.bindValue(2, window->getEntries());
            insertIntoMoviesQuery.bindValue(3, window->getRating());

            movieName = window->getName();
            movieYear = QString::number(window->getReleaseYear());
        }
        else {
            movieName = window->getComboboxSelectedItem().remove(window->getComboboxSelectedItem().length()-7, window->getComboboxSelectedItem().length());
            movieYear = window->getComboboxSelectedItem().remove(0, window->getComboboxSelectedItem().length()-4);
        }

        if(!insertIntoMoviesQuery.exec()){
            m_log->append(tr("Erreur lors de l'ajout dans la table movies, plus d'informations ci-dessous :\nCode d'erreur ")+insertIntoMoviesQuery.lastError().nativeErrorCode()+tr(" : ")+insertIntoMoviesQuery.lastError().text());
        }

        //Add the view to the views table
        QSqlQuery insertIntoViewsQuery;
        insertIntoViewsQuery.prepare("INSERT INTO views (ID_Movie, ViewDate, ViewType) VALUES (?,?,?);");

        int viewedMovieID = -1;


        QSqlQuery viewedMovieIDQuery;
        viewedMovieIDQuery.exec("SELECT ID FROM movies WHERE Name=\""+movieName+"\" AND ReleaseYear=\""+movieYear+"\"");
        viewedMovieIDQuery.first();

        viewedMovieID = viewedMovieIDQuery.value(0).toInt();

        QString ViewDate, ViewType;

        if(window->isDateUnknown()) {
            ViewDate = "?";
        }
        else {
            ViewDate = window->getViewDate();
        }

        if(window->isTypeUnknown()) {
            ViewType = "?";
        }
        else {
            ViewType = window->getViewType();
        }

        insertIntoViewsQuery.bindValue(0, viewedMovieID);
        insertIntoViewsQuery.bindValue(1, ViewDate);
        insertIntoViewsQuery.bindValue(2, ViewType);

        if(!insertIntoViewsQuery.exec()){
            m_log->append(tr("Erreur lors de l'ajout dans la table movies, plus d'informations ci-dessous :\nCode d'erreur ")+insertIntoViewsQuery.lastError().nativeErrorCode()+tr(" : ")+insertIntoViewsQuery.lastError().text());
        }

        fillGlobalStats();
        loadDB();
    }
}

void MainWindow::editViews() {
    int ID = m_ui->MoviesListWidget->item(m_ui->MoviesListWidget->currentRow(),7)->text().toInt();
    EditViewsDialog* window = new EditViewsDialog(&ID, m_log, &m_theme);
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
        refreshTheme();
        saveSettings();
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

void MainWindow::customMenuRequested(QPoint pos) {
    QMenu *menu = new QMenu(this);

    QAction* deleteAction = new QAction(tr("Supprimer"), this);
    if(m_theme == Theme::Classic) {
        deleteAction->setIcon(QIcon(":/icons/Icons/remove.png"));
    }
    else {
        deleteAction->setIcon(QIcon(":/icons/Icons/remove light.png"));
    }

    QAction* editAction = new QAction(tr("Modifier"), this);
    if(m_theme == Theme::Classic) {
        editAction->setIcon(QIcon(":/icons/Icons/edit.png"));
    }
    else {
        editAction->setIcon(QIcon(":/icons/Icons/edit light.png"));
    }

    menu->addAction(editAction);
    menu->addAction(deleteAction);

    QObject::connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteMovie()));
    QObject::connect(editAction, SIGNAL(triggered()), this, SLOT(editMovie()));
    menu->popup(m_ui->MoviesListWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::editMovie() {
    EditMovieDialog* window = new EditMovieDialog(m_ui->MoviesListWidget);
    window->show();
    if(window->exec() == 1) {

        QString ID = m_ui->MoviesListWidget->item(m_ui->MoviesListWidget->currentRow(),7)->text();

        QSqlQuery editMovieQuery;
        if(!editMovieQuery.exec("UPDATE movies SET Name=\""+window->getMovieName()+"\", ReleaseYear=\""+window->getReleaseYear()+"\", Entries=\""+QString::number(window->getEntries())+"\", Rating=\""+QString::number(window->getRating())+"\" WHERE ID=\""+ID+"\";")) {
            m_log->append(tr("Erreur lors de l'édition dans la table movies, plus d'informations ci-dessous :\nCode d'erreur ")+editMovieQuery.lastError().nativeErrorCode()+tr(" : ")+editMovieQuery.lastError().text());
        }
        loadDB();
    }
}

void MainWindow::deleteMovie() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Supprimer le film"), tr("Êtes-vous sûr de vouloir supprimer le film ? Les visionnages associés seront effacés."));
    if(reply == QMessageBox::Yes) {
        QSqlQuery deleteMovieQuery;
        QSqlQuery deleteAssociatedViewsQuery;
        QString ID = m_ui->MoviesListWidget->item(m_ui->MoviesListWidget->currentRow(),7)->text();

        if(!deleteMovieQuery.exec("DELETE FROM movies WHERE ID=\""+ID+"\";")) {
            m_log->append(tr("Erreur lors de la suppression dans la table movies, plus d'informations ci-dessous :\nCode d'erreur ")+deleteMovieQuery.lastError().nativeErrorCode()+tr(" : ")+deleteMovieQuery.lastError().text());
        }

        if(!deleteAssociatedViewsQuery.exec("DELETE FROM views WHERE ID_Movie=\""+ID+"\";")) {
            m_log->append(tr("Erreur lors de la suppression dans la table movies, plus d'informations ci-dessous :\nCode d'erreur ")+deleteMovieQuery.lastError().nativeErrorCode()+tr(" : ")+deleteMovieQuery.lastError().text());
        }

        resetFilters();
        fillGlobalStats();
    }
}

void MainWindow::menuBarConnectors() {
    QObject::connect(m_ui->QuitAct, SIGNAL(triggered()), m_app, SLOT(quit()));
    QObject::connect(m_ui->LogAct, SIGNAL(triggered()), this, SLOT(openLog()));
    QObject::connect(m_ui->AboutAct, SIGNAL(triggered()), this, SLOT(openAbout()));
    QObject::connect(m_ui->OptionsAct, SIGNAL(triggered()), this, SLOT(openSettings()));
    QObject::connect(m_ui->ImportAct, SIGNAL(triggered()), this, SLOT(importDB()));
    QObject::connect(m_ui->ExportAct, SIGNAL(triggered()), this, SLOT(exportDB()));
    QObject::connect(m_ui->ChartAct, SIGNAL(triggered()), this, SLOT(openCharts()));
}

void MainWindow::setSettings() {
    m_language = m_settings->value("language").toInt();
    m_theme = m_settings->value("theme").toInt();
    m_matrixMode = m_settings->value("matrixMode").toBool();
}

void MainWindow::saveSettings() {
    m_settings->setValue("language", m_language);
    m_settings->setValue("theme", m_theme);
    m_settings->setValue("matrixMode", m_matrixMode);
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
    fillGlobalStats();
}

void MainWindow::refreshTheme() {
    QString path;

    switch(m_theme) {
        case Theme::Classic :
            path = ":/styles/Styles/classic.qss";
            break;
        case Theme::Dark :
            path = ":/styles/Styles/dark.qss";
            break;
        case Theme::MidnightPurple :
            path = ":/styles/Styles/midnightPurple.qss";
            break;
        case Theme::OLED :
            path = ":/styles/Styles/oled.qss";
            break;
    }

    QFile qss(path);
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
}

void MainWindow::fillGlobalStats() {

    QSqlQuery uniqueViewQuery;
    uniqueViewQuery.exec("SELECT COUNT(*) FROM movies;");
    uniqueViewQuery.first();

    QSqlQuery totalViewQuery;
    totalViewQuery.exec("SELECT COUNT(*) FROM views;");
    totalViewQuery.first();

    QSqlQuery avgMovieYearQuery;
    avgMovieYearQuery.exec("SELECT ReleaseYear, Rating FROM movies");
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
    movieThisYearQuery.exec("SELECT * FROM views WHERE ViewDate BETWEEN '"+QString::number(QDate::currentDate().year())+"-01-01' AND '"+QString::number(QDate::currentDate().year())+"-12-31' GROUP BY ID_Movie;");
    while(movieThisYearQuery.next()) {
        movieThisYear++;
    }



    m_ui->TotalUniqueViewsLabel->setText(tr("Nombre de vues uniques : ") + uniqueViewQuery.value(0).toString());
    m_ui->TotalViewLabel->setText(tr("Nombre total de visionnages : ") + totalViewQuery.value(0).toString());
    m_ui->AverageViewLabel->setText(tr("Moyenne de visionnages : ") + QString::number(avgViews));
    m_ui->AverageYearLabel->setText(tr("Année moyenne des films vus : ") + QString::number(avgMovieYear));
    m_ui->ViewThisYear->setText(tr("Vues cette année : ") + QString::number(movieThisYear));
    m_ui->AverageRatingLabel->setText(tr("Note moyenne : ") + QString::number(avgRating));
}

void MainWindow::openCharts() {
    if(ChartsDialog::instancesCount() == 0) {
        ChartsDialog* window = new ChartsDialog(this);
        window->show();
        if(window->exec() == 0) {
            delete window;
        }
    }
}

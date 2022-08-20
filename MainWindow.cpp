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

    QObject::connect(m_ui->AddViewButton, SIGNAL(clicked()), this, SLOT(addView()));
    QObject::connect(m_ui->ManageMovieViewsButton, SIGNAL(clicked()), this, SLOT(editViews()));
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
                         "WHERE Name LIKE \"%" + m_filter_name + "%\""
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
        viewsQuery.exec("SELECT COUNT(*) FROM movieViews WHERE Name=\""+moviesQuery.value(0).toString()+"\" AND ReleaseYear='"+moviesQuery.value(1).toString()+"' AND Entries='"+moviesQuery.value(2).toString()+"' AND Rating='"+moviesQuery.value(3).toString()+"'");
        viewsQuery.first();
        numberOfViews->setText(viewsQuery.value(0).toString());

        //Fetch the first view of the current movie
        QSqlQuery firstViewQuery;
        firstViewQuery.exec("SELECT ViewDate FROM movieViews WHERE Name=\""+moviesQuery.value(0).toString()+"\" AND ReleaseYear="+moviesQuery.value(1).toString()+" AND Entries="+moviesQuery.value(2).toString()+" AND Rating="+moviesQuery.value(3).toString()+" AND NOT ViewDate='?' ORDER BY ViewDate ASC LIMIT 1");
        firstViewQuery.first();
        firstSeen->setText(firstViewQuery.value(0).toString());

        //Fetch the last view of the current movie
        QSqlQuery lastViewQuery;
        lastViewQuery.exec("SELECT ViewDate FROM movieViews WHERE Name=\""+moviesQuery.value(0).toString()+"\" AND ReleaseYear="+moviesQuery.value(1).toString()+" AND Entries="+moviesQuery.value(2).toString()+" AND Rating="+moviesQuery.value(3).toString()+" AND NOT ViewDate='?' ORDER BY ViewDate DESC LIMIT 1");
        lastViewQuery.first();
        lastSeen->setText(lastViewQuery.value(0).toString());

        QSqlQuery hasUnknownView;
        hasUnknownView.exec("SELECT ViewDate FROM movieViews WHERE Name=\""+moviesQuery.value(0).toString()+"\" AND ReleaseYear="+moviesQuery.value(1).toString()+" AND Entries="+moviesQuery.value(2).toString()+" AND Rating="+moviesQuery.value(3).toString()+" AND ViewDate='?'");
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

    //Disable Manage views and filters button if no movie the list is empty
    if(m_ui->MoviesListWidget->currentRow() == -1) {
        m_ui->ManageMovieViewsButton->setEnabled(false);
        m_ui->AdvancedSearchButton->setEnabled(false);
    }
    else {
        m_ui->ManageMovieViewsButton->setEnabled(true);
        m_ui->AdvancedSearchButton->setEnabled(true);
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
                    deleteQuery.exec("DELETE FROM movieViews");
                }
                else {
                    add = false;
                }
            }
            if(add) {
                QString val = jsonFile.readAll();
                jsonFile.close();
                QJsonObject movies = QJsonDocument::fromJson(val.toUtf8()).object();
                foreach(const QString& key, movies.keys()) {
                    QJsonObject movie = movies.value(key).toObject();

                    QSqlQuery query;
                    query.prepare("INSERT INTO movieViews (Name, ReleaseYear, ViewDate, ViewType, Rating, Entries) VALUES (?,?,?,?,?,?);");
                    query.bindValue(0, movie["Name"].toString());
                    query.bindValue(1, movie["ReleaseYear"].toInt());
                    query.bindValue(2, movie["ViewDate"].toString());
                    query.bindValue(3, movie["ViewType"].toString());
                    query.bindValue(4, movie["Rating"].toInt());
                    query.bindValue(5, movie["Entries"].toInt());

                    if(!query.exec()){
                        m_log->append(tr("Erreur lors de l'ajout dans la base de données, plus d'informations ci-dessous :\nCode d'erreur ")+query.lastError().nativeErrorCode()+tr(" : ")+query.lastError().text());
                    }

                }

            }
        }
    }
    fillGlobalStats();
    loadDB();
}

void MainWindow::exportDB() {
    QString file = QFileDialog::getSaveFileName(this, tr("Exporter"), QString(), "JSON (*.json)");  //Get the save link
    //Creates a QFile with the fetched path
    QFile jsonFile(file);
    //Test if the file is correctly opened
    if (!jsonFile.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Erreur"), jsonFile.errorString());
    }

    QJsonObject moviesObject;

    QSqlQuery moviesQuery;
    moviesQuery.exec("SELECT Name, ReleaseYear, ViewDate, Entries, Rating, ViewType FROM movieViews;");
    int i=0;
    while(moviesQuery.next()) {
        i++;

        QJsonObject movieObject;

        movieObject.insert("Name",QJsonValue::fromVariant(moviesQuery.value(0).toString()));
        movieObject.insert("ReleaseYear",QJsonValue::fromVariant(moviesQuery.value(1).toInt()));
        movieObject.insert("ViewDate",QJsonValue::fromVariant(moviesQuery.value(2).toString()));
        movieObject.insert("Entries",QJsonValue::fromVariant(moviesQuery.value(3).toInt()));
        movieObject.insert("Rating",QJsonValue::fromVariant(moviesQuery.value(4).toInt()));
        movieObject.insert("ViewType",QJsonValue::fromVariant(moviesQuery.value(5).toString()));

        moviesObject.insert("movie" + QString::fromStdString(std::to_string(i)), movieObject);
    }
    jsonFile.write(QJsonDocument(moviesObject).toJson(QJsonDocument::Indented));
    jsonFile.close();
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
            ratingQuery.exec("SELECT Rating, Entries FROM movieViews WHERE Name=\""+movieName+"\" AND ReleaseYear=\""+movieYear+"\" GROUP BY Rating");
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
    deleteAction->setIcon(QIcon(":/icons/Icons/remove.png"));

    QAction* editAction = new QAction(tr("Modifier"), this);
    editAction->setIcon(QIcon(":/icons/Icons/edit.png"));

    menu->addAction(deleteAction);
    menu->addAction(editAction);

    QObject::connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteMovie()));
    QObject::connect(editAction, SIGNAL(triggered()), this, SLOT(editMovie()));
    menu->popup(m_ui->MoviesListWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::editMovie() {

}

void MainWindow::deleteMovie() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Supprimer le film"), tr("Êtes-vous sûr de vouloir supprimer le film ? Les visionnages associés seront effacés."));
    if(reply == QMessageBox::Yes) {
        QSqlQuery deleteMovieQuery;
        QString name = m_ui->MoviesListWidget->item(m_ui->MoviesListWidget->currentRow(),0)->text();
        QString releaseYear = m_ui->MoviesListWidget->item(m_ui->MoviesListWidget->currentRow(),1)->text();
        QString entriesFR = m_ui->MoviesListWidget->item(m_ui->MoviesListWidget->currentRow(),5)->text();
        QString rating = m_ui->MoviesListWidget->item(m_ui->MoviesListWidget->currentRow(),6)->text();

        deleteMovieQuery.exec("DELETE FROM movieViews WHERE Name=\""+name+"\" AND ReleaseYear=\""+releaseYear+"\" AND Entries=\""+entriesFR+"\" AND Rating=\""+rating+"\";");

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



    m_ui->TotalUniqueViewsLabel->setText(tr("Nombre de vues uniques : ") + QString::number(m_ui->MoviesListWidget->rowCount()));
    m_ui->TotalViewLabel->setText(tr("Nombre total de visionnages : ") + totalViewQuery.value(0).toString());
    m_ui->AverageViewLabel->setText(tr("Moyenne de visionnages : ") + QString::number(avgViews));
    m_ui->AverageYearLabel->setText(tr("Année moyenne des films vus : ") + QString::number(avgMovieYear));
    m_ui->ViewThisYear->setText(tr("Vues cette année : ") + QString::number(movieThisYear));
    m_ui->AverageRatingLabel->setText(tr("Note moyenne : ") + QString::number(avgRating));
}

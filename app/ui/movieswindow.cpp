#include "movieswindow.h"
#include "mainwindow.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIntValidator>
#include <QSettings>
#include <QMessageBox>
#include <QGuiApplication>
#include <QHeaderView>
#include "qtreeview.h"
#include <QTreeWidget>
#include "qtorrent.h"
#include "addtorrentdialog.h"
#include "core/torrent.h"
#include "core/torrentinfo.h"
#include "core/torrentmanager.h"
#include <QToolButton>
#include <QKeyEvent>

MoviesWindow::MoviesWindow(QWidget *parent)
    : QWidget(parent)
{
    // Connect to sqlite database
    m_dbInterface = DBInterface();

    //Initialize movies window
    initMoviesLayout();

    //Set movies window styles
    setStyles();

    // Populate genre combo box with possible values selected from database
//    QStringList genreNames = m_dbInterface.getGenres();
//    foreach(QString name, genreNames)
//        comboGenre->addItem(name);

    // Connect control signals to slots
    connect(treeMovies, SIGNAL(itemSelectionChanged()), this, SLOT(refreshMovieDetails()));
//    connect(lineTitle, SIGNAL(editingFinished()), this, SLOT(updateMovie()));
//    connect(comboGenre, SIGNAL(currentTextChanged(QString)), this, SLOT(updateMovie()));
//    connect(spinboxYear, SIGNAL(editingFinished()), this, SLOT(updateMovie()));
//    connect(doublespinRating, SIGNAL(editingFinished()), this, SLOT(updateMovie()));
//    connect(lineActors, SIGNAL(editingFinished()), this, SLOT(updateMovie()));
    connect(downloadButton, &QPushButton::clicked, this, &MoviesWindow::addTorrentFromUrl);
    connect(imageButton, &QPushButton::clicked, this, &MoviesWindow::addTorrentFromUrl);
    //connect(btnSearchPrevious, SIGNAL(clicked()), this, SLOT(searchPrevious()));
    //connect(btnSearchNext, SIGNAL(clicked()), this, SLOT(searchNext()));
    connect(searchButton, SIGNAL(clicked()), this, SLOT(anotherSearch()));

    // Update movie treeWidget
    refreshMovies();

    // Select the first movie in the tree widget
    treeMovies->setCurrentItem(treeMovies->topLevelItem(0));
}

void MoviesWindow::initMoviesLayout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 5);

    QVBoxLayout *movieTreeLayout = new QVBoxLayout;
    QHBoxLayout *searchLayout = new QHBoxLayout;
    movieTreeLayout->addWidget(treeMovies = new QTreeWidget());
    movieTreeLayout->addLayout(searchLayout);
    treeMovies->setIndentation(0);
    treeMovies->setSortingEnabled(true);
    movieTreeLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *movieInfoLayout = new QVBoxLayout;
    movieInfoLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addLayout(movieTreeLayout);
    mainLayout->addLayout(movieInfoLayout);

    QVBoxLayout *notImageLayout = new QVBoxLayout;
    QHBoxLayout *ImageBtnLayout = new QHBoxLayout;
    QHBoxLayout *downloadBtnLayout = new QHBoxLayout;
    QHBoxLayout *titleLayout = new QHBoxLayout;
    QHBoxLayout *yearLayout = new QHBoxLayout;
    QHBoxLayout *genreLayout = new QHBoxLayout;
    QHBoxLayout *ratingLayout = new QHBoxLayout;
    QHBoxLayout *actorsLayout = new QHBoxLayout;


    //searchLayout->addWidget(labelSearch = new QLabel);
    searchLayout->addWidget(comboSearchField = new QComboBox);
    searchLayout->addWidget(lineSearchTerm = new QLineEdit);
    //searchLayout->addWidget(btnSearchPrevious = new QToolButton);
    //searchLayout->addWidget(btnSearchNext = new QToolButton);
    searchLayout->addWidget(searchButton = new QPushButton);


    // Populate search field combo box with possible values
    // Note: these values must be added in numerical order
    comboSearchField->addItem("Title");  // MOVIE_TITLE_COL  = 0
    comboSearchField->addItem("Year");   // MOVIE_YEAR_COL   = 1
    comboSearchField->addItem("Genre");  // MOVIE_GENRE_COL  = 2
    comboSearchField->addItem("Rating"); // MOVIE_RATING_COL = 3
    comboSearchField->addItem("Actors"); // MOVIE_ACTORS_COL = 4

    titleLayout->addWidget(m_title = new QLabel);
    yearLayout->addWidget(m_year = new QLabel);
    genreLayout->addWidget(m_genre = new QLabel);
    ratingLayout->addWidget(m_rating = new QLabel);
    actorsLayout->addWidget(m_actors = new QLabel);
    titleLayout->addWidget(lineTitle = new QLineEdit);
    yearLayout->addWidget(spinboxYear = new QLineEdit);
    genreLayout->addWidget(comboGenre = new QLineEdit);
    ratingLayout->addWidget(doublespinRating = new QLineEdit);
    actorsLayout->addWidget(lineActors = new QLineEdit);

    ImageBtnLayout->addWidget(imageButton = new QPushButton);

    notImageLayout->addLayout(titleLayout);
    notImageLayout->addLayout(yearLayout);
    notImageLayout->addLayout(genreLayout);
    notImageLayout->addLayout(ratingLayout);
    notImageLayout->addLayout(actorsLayout);
    notImageLayout->setContentsMargins(5, 0, 5, 0);

    downloadBtnLayout->addWidget(downloadButton = new QPushButton);
    notImageLayout->addLayout(downloadBtnLayout);
    notImageLayout->setAlignment(Qt::AlignHCenter);
    notImageLayout->setAlignment(Qt::AlignBottom);

    movieInfoLayout->addLayout(ImageBtnLayout);
    movieInfoLayout->addLayout(notImageLayout);

    mainLayout->setStretchFactor(movieInfoLayout, 30);
    mainLayout->setStretchFactor(movieTreeLayout, 70);
    movieInfoLayout->setStretchFactor(notImageLayout, 20);
    movieInfoLayout->setStretchFactor(ImageBtnLayout, 80);
    titleLayout->setStretchFactor(m_title, 25);
    titleLayout->setStretchFactor(lineTitle, 85);
    yearLayout->setStretchFactor(m_year, 25);
    yearLayout->setStretchFactor(spinboxYear, 85);
    ratingLayout->setStretchFactor(m_rating, 25);
    ratingLayout->setStretchFactor(doublespinRating, 85);
    genreLayout->setStretchFactor(m_genre, 25);
    genreLayout->setStretchFactor(comboGenre, 85);
    actorsLayout->setStretchFactor(m_actors, 25);
    actorsLayout->setStretchFactor(lineActors, 85);
    movieTreeLayout->setStretchFactor(treeMovies, 80);
    movieTreeLayout->setStretchFactor(searchLayout, 40);
    searchLayout->setStretchFactor(comboSearchField, 20);
    searchLayout->setStretchFactor(lineSearchTerm, 60);
    searchLayout->setStretchFactor(searchButton, 15);
}

void MoviesWindow::setStyles()
{
    //spinboxYear->setRange(1900, 2020);
    //spinboxYear->setValue(2000);
    //doublespinRating->setRange(0.0, 10.0);
    //doublespinRating->setDecimals(1);

    imageButton->setStyleSheet("    background-color: #414141;"
                               "    border-image: url(:/default);");
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(imageButton->sizePolicy().hasHeightForWidth());
    imageButton->setSizePolicy(sizePolicy);

    QCursor cursor = downloadButton->cursor();
    cursor.setShape(Qt::OpenHandCursor);
    downloadButton->setCursor(cursor);
    imageButton->setCursor(cursor);
    comboSearchField->setCursor(cursor);
    lineSearchTerm->setPlaceholderText("Search file...");


    this->setStyleSheet("QLineEdit {"
                        "color: #414141;"
                        "border: 2px solid gray;"
                        "border-radius: 10px;"
                        "padding: 0 8px;"
                        "background: rgb(244, 244, 244);"
                        "selection-background-color: darkgray;"
                        "font-size: 14px;"
                        "}"
                        "QComboBox {"
                        "color: #414141;"
                        "border: 2px solid gray;"
                        "border-radius: 10px;"
                        "padding: 0 8px;"
                        "background: rgb(244, 244, 244);"
                        "selection-background-color: darkgray;"
                        "font-size: 14px;"
                        "}"
                        "QDoubleSpinBox {"
                        "color: #414141;"
                        "border: 2px solid gray;"
                        "border-radius: 10px;"
                        "padding: 0 8px;"
                        "background: rgb(244, 244, 244);"
                        "selection-background-color: darkgray;"
                        "font-size: 14px;"
                        "}"
                        "QSpinBox {"
                        "color: #414141;"
                        "border: 2px solid gray;"
                        "border-radius: 10px;"
                        "padding: 0 8px;"
                        "background: rgb(244, 244, 244);"
                        "selection-background-color: darkgray;"
                        "font-size: 14px;"
                        "}"
                        "QTreeWidget {"
                        "font-size: 14px;"
                        "show-decoration-selected: 1;"
                        "}"
                        "QTreeWidget::item {"
                        "border: 1px solid #d9d9d9;"
                        "border-top-color: transparent;"
                        "border-bottom-color: transparent;"
                        "}"
                        "QTreeWidget::item:selected {"
                        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 orange, stop: 1 #cbdaf1);"
                        "border: 1px solid #bfcde4;"
                        "color: #414141;"
                        "}"
                        "QTreeView::item:hover {"
                        "border: 1px solid #414141;"
                        "color: #414141;"
                        "}"
                        "QLabel {"
                        "font-size: 18px;"
                        "font-family: Arial;"
                        "font-weight: 600;"
                        "font: italic;"
                        "}"
                        "QComboBox {"
                        "border: 1px solid gray;"
                        "border-radius: 10px;"
                        "padding: 1px 18px 1px 3px;"
                        "min-width: 6em;"
                        "font-size: 16px;"
                        "font-family: Arial;"
                        "font-weight: 600;"
                        "background-color: orange;"
                        "}"
                        "QComboBox::item {"
                        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 orange, stop: 1 #cbdaf1);"
                        "}"
                        "QComboBox:editable {"
                        "background: white;"
                        "}"
                        "QComboBox:!editable, QComboBox::drop-down:editable {"
                        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                        "stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,"
                        "stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);"
                        "}"
                        "QComboBox:!editable:on, QComboBox::drop-down:editable:on {"
                        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                        "stop: 0 #D3D3D3, stop: 0.4 #D8D8D8,"
                        "stop: 0.5 #DDDDDD, stop: 1.0 #E1E1E1);"
                        "}"
                        "QComboBox:on {"
                        "padding-top: 3px;"
                        "padding-left: 4px;"
                        "}"
                        "QComboBox::drop-down {"
                        "subcontrol-origin: padding;"
                        "subcontrol-position: top right;"
                        "width: 15px;"
                        "border-left-width: 1px;"
                        "border-left-color: darkgray;"
                        "border-left-style: solid;"
                        "border-top-right-radius: 3px;"
                        "border-bottom-right-radius: 3px;"
                        "}"
                        "QComboBox::down-arrow {"
                        "image: url(:/dropdownarrow.png);"
                        "}");

    downloadButton->setStyleSheet("QPushButton {"
                                  "background-color: orange;"
                                  "border-style: outset;"
                                  "border-width: 2px;"
                                  "border-radius: 10px;"
                                  "border-color: black;"
                                  "font: bold 14px;"
                                  "min-width: 10em;"
                                  "padding: 6px;"
                                  "}"
                                  "QPushButton:pressed {"
                                  "background-color: green;"
                                  "border-style: inset;"
                                  "}");
    searchButton->setText("Search");
    searchButton->setStyleSheet("QPushButton {"
                                "background-color: orange;"
                                "border-style: outset;"
                                "border-width: 2px;"
                                "border-radius: 10px;"
                                "border-color: black;"
                                "font: bold 14px;"
                                "min-width: 10em;"
                                "padding: 6px;"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: green;"
                                "border-style: inset;"
                                "}");
    searchButton->setMinimumHeight(40);
    lineSearchTerm->setMinimumHeight(40);
    comboSearchField->setMinimumHeight(40);

    downloadButton->setText("Download");

    m_title->setText("Title");
    m_actors->setText("Actors");
    m_year->setText("Year");
    m_genre->setText("Genre");
    m_rating->setText("Rating");


    // Set up the movie TreeWidget
    treeMovies->setColumnCount(7);
    treeMovies->hideColumn(5);
    treeMovies->hideColumn(6);
    treeMovies->setHeaderLabels(QStringList() << "Title" << "Year" << "Genre" << "Rating" << "Actors");
    treeMovies->setStyleSheet("color: #414141;"
                              "background-color: #c0c0c0;");

    // Set the MainWindow size and position
    QSettings settings;
    this->restoreGeometry(settings.value(MAINWINDOW_GEOMETRY,"").toByteArray());

    // Set TreeWidget column widths
    treeMovies->setColumnWidth(0, settings.value(TREE_WIDTH_0,"300").toInt());
    treeMovies->setColumnWidth(1, settings.value(TREE_WIDTH_1,"50").toInt());
    treeMovies->setColumnWidth(2, settings.value(TREE_WIDTH_2,"100").toInt());
    treeMovies->setColumnWidth(3, settings.value(TREE_WIDTH_3,"50").toInt());
    treeMovies->setColumnWidth(4, settings.value(TREE_WIDTH_4,"300").toInt());

    lineTitle->setReadOnly(true);
    lineActors->setReadOnly(true);
    //comboGenre->setEditable(false);
    comboGenre->setReadOnly(true);
    doublespinRating->setReadOnly(true);
    spinboxYear->setReadOnly(true);
}

void MoviesWindow::refreshMovies()
{
    // Clear the movies tree widget
    treeMovies->clear();
    // Fetch movie details and populate the tree widget
    QList<MovieData> movieList = m_dbInterface.getMovies();
    foreach(MovieData movieData, movieList)
    {
        QTreeWidgetItem *movieItem = new QTreeWidgetItem(treeMovies);
        movieItem->setText(MOVIE_ID_COL, QString::number(movieData.id));
        movieItem->setText(MOVIE_TITLE_COL, movieData.title);
        movieItem->setText(MOVIE_YEAR_COL, movieData.year);
        movieItem->setText(MOVIE_GENRE_COL, movieData.genre);
        movieItem->setText(MOVIE_RATING_COL, movieData.rating);
        movieItem->setText(MOVIE_ACTORS_COL, movieData.actors);
        movieItem->setText(MOVIE_IMAGE_COL, movieData.image);
    }
}

//void MoviesWindow::updateMovie()
//{
//    // Get the current movie ID
//    QTreeWidgetItem *currentMovieItem = treeMovies->currentItem();
//    if (currentMovieItem==NULL)
//        return;
//    int movieId = currentMovieItem->data(MOVIE_ID_COL, Qt::DisplayRole).toInt();

//    // Get new values
//    QString title = lineTitle->text();
//    //int genreId = comboGenre->currentIndex()+1;
//    QString genre = comboGenre->text();
//    QString year = spinboxYear->text();
//    QString rating = doublespinRating->text();
//    QString actors = lineActors->text();

//    // Update the database
//    //m_dbInterface.updateMovie(movieId, title, genreId, year, rating, actors);

//    // Update the movie tree model
//    currentMovieItem->setData(MOVIE_TITLE_COL,Qt::DisplayRole, title);
//    currentMovieItem->setData(MOVIE_GENRE_COL,Qt::DisplayRole, genre);
//    currentMovieItem->setData(MOVIE_YEAR_COL,Qt::DisplayRole, year);
//    currentMovieItem->setData(MOVIE_RATING_COL,Qt::DisplayRole, rating);
//    currentMovieItem->setData(MOVIE_ACTORS_COL,Qt::DisplayRole, actors);
//}

void MoviesWindow::refreshMovieDetails()
{
    // Get the current item
    QTreeWidgetItem* currentItem = treeMovies->currentItem();
    QString imageName = currentItem->data(MOVIE_IMAGE_COL, Qt::DisplayRole).toString();

    // Clear detail controls
    lineTitle->clear();
    spinboxYear->clear();
    doublespinRating->clear();
    lineActors->clear();

    if (currentItem == NULL)
        return;

    // Populate detail controls
    lineTitle->setText(currentItem->data(MOVIE_TITLE_COL, Qt::DisplayRole).toString());
    comboGenre->blockSignals(true);
    comboGenre->setText(currentItem->data(MOVIE_GENRE_COL, Qt::DisplayRole).toString());
    comboGenre->blockSignals(false);
    spinboxYear->setText(currentItem->data(MOVIE_YEAR_COL, Qt::DisplayRole).toString());
    doublespinRating->setText(currentItem->data(MOVIE_RATING_COL, Qt::DisplayRole).toString());
    lineActors->setText(currentItem->data(MOVIE_ACTORS_COL, Qt::DisplayRole).toString());
    imageButton->setStyleSheet(QString("    background-color: #414141;"
                               "    border-image: url(:/%1.jpg)").arg(imageName));
}


void MoviesWindow::addTorrentFromUrl()
{
    QTreeWidgetItem* currentItem = treeMovies->currentItem();
    QString imageName = currentItem->data(MOVIE_IMAGE_COL, Qt::DisplayRole).toString();
    QUrl url;
    url.setUrl(QString("file:D:/QtProjects/TorrentPart/Bambooster/res/torrents/%1.torrent").arg(imageName));
    //url.setUrl(QString(":/%1.torrent").arg(imageName));

    MainWindow::m_mainWindow->addTorrentFromUrl(url);
}

//void MoviesWindow::addMovie()
//{
//    // Default data for the new movie
//    QString title("NewMovie");
//    int year = 2018;
//    int genreId = 1;
//    QString genre("Action");
//    double rating = 5.0;

//    // Insert a new movie record in the database
//    int movieId = m_dbInterface.insertMovie(title, genreId, year, rating);

//    // Add the new movie to the movie tree view
//    QTreeWidgetItem *movieItem = new QTreeWidgetItem(treeMovies);
//    movieItem->setText(MOVIE_ID_COL, QString::number(movieId));
//    movieItem->setText(MOVIE_TITLE_COL, title);
//    movieItem->setText(MOVIE_YEAR_COL, QString::number(year));
//    movieItem->setText(MOVIE_GENRE_COL, genre);
//    movieItem->setText(MOVIE_RATING_COL, QString::number(rating));

//    // Select the newly created movie in the tree view so the user can edit it
//    int numItems = treeMovies->topLevelItemCount();
//    treeMovies->setCurrentItem(treeMovies->topLevelItem(numItems-1));
//}

//void MoviesWindow::deleteMovie()
//{
//    // Get the current movie ID
//    QTreeWidgetItem *currentMovieItem = treeMovies->currentItem();
//    if (currentMovieItem==NULL)
//        return;
//    int movieId = currentMovieItem->data(MOVIE_ID_COL, Qt::DisplayRole).toInt();

//    // Delete the movie from the database
//    m_dbInterface.deleteMovie(movieId);

//    // Remove the tree item widget from the movie tree
//    delete currentMovieItem;
//}

void MoviesWindow::searchPrevious()
{
    // Search up the widget
    bool searchDown = false;
    search(searchDown);
}

void MoviesWindow::searchNext()
{
    // Search down the widget
    bool searchDown = true;
    search(searchDown);

}

void MoviesWindow::anotherSearch()
{
    // Clear the movies tree widget
    treeMovies->clear();

    QString searchTerm = lineSearchTerm->text().toUpper();
    int columnNumber = comboSearchField->currentIndex();

    // Fetch movie details and populate the tree widget
    QList<MovieData> movieList = m_dbInterface.getMovies();
    foreach(MovieData movieData, movieList)
    {
        switch(columnNumber){
        case 0:
            fieldContents = movieData.title.toUpper();
            break;
        case 1:
            fieldContents = movieData.year.toUpper();
            break;
        case 2:
            fieldContents = movieData.genre.toUpper();
            break;
        case 3:
            fieldContents = movieData.rating.toUpper();
            break;
        case 4:
            fieldContents = movieData.actors.toUpper();
            break;
        }
        if (fieldContents.contains(searchTerm))
        {
            QTreeWidgetItem *movieItem = new QTreeWidgetItem(treeMovies);
            movieItem->setText(MOVIE_ID_COL, QString::number(movieData.id));
            movieItem->setText(MOVIE_TITLE_COL, movieData.title);
            movieItem->setText(MOVIE_YEAR_COL, movieData.year);
            movieItem->setText(MOVIE_GENRE_COL, movieData.genre);
            movieItem->setText(MOVIE_RATING_COL, movieData.rating);
            movieItem->setText(MOVIE_ACTORS_COL, movieData.actors);
            movieItem->setText(MOVIE_IMAGE_COL, movieData.image);
        }
    }
}

void MoviesWindow::search(bool searchDown)
{
    // Get starting values
    QString searchTerm = lineSearchTerm->text();
    QTreeWidgetItem *currentItem = treeMovies->currentItem();
    QTreeWidgetItem *startItem = currentItem;
    currentItem = getNextItem(currentItem, searchDown);
    int columnNumber = comboSearchField->currentIndex();

    // Walk the widget until we find an item that meets the search criteria
    bool itemFound = false;
    while(!itemFound && currentItem != startItem)
    {
        QString fieldContents = currentItem->data(columnNumber, Qt::DisplayRole).toString();
        if (fieldContents.contains(searchTerm))
        {
            itemFound = true;
        }
        if (!itemFound)
        {
            currentItem = getNextItem(currentItem, searchDown);
        }
    }

    // If we found an item, select it
    if (itemFound)
    {
        treeMovies->setCurrentItem(currentItem);
    }
}

QTreeWidgetItem* MoviesWindow::getNextItem(QTreeWidgetItem* currentItem, bool searchDown)
{
    QTreeWidgetItem *nextItem = NULL;
    if (searchDown)
    {
        nextItem = treeMovies->itemBelow(currentItem);
        if (nextItem == NULL)
        {
            // wrap around to the top of the list
            nextItem = treeMovies->topLevelItem(0);
        }
    }
    else
    {
        nextItem = treeMovies->itemAbove(currentItem);
        if (nextItem == NULL)
        {
            // wrap around to the bottom of the list
            int numItems = treeMovies->topLevelItemCount();
            nextItem = treeMovies->topLevelItem(numItems-1);
        }
    }

    return nextItem;
}

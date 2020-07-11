#ifndef MOVIESWINDOW_H
#define MOVIESWINDOW_H


#include <QWidget>
#include <QTreeWidgetItem>
#include "panel.h"
#include <QTreeWidget>
#include <QList>
#include "dbinterface.h"
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QIcon>

class MoviesWindow : public QWidget
{
    Q_OBJECT

public:
    MoviesWindow(QWidget *parent = nullptr);

private slots:
    // Screen updates
    void refreshMovies();
    void refreshMovieDetails();

    // Edit data
    //void addMovie();
    //void updateMovie();
    //void deleteMovie();

    //Init
    void initMoviesLayout();
    void setStyles();

    // Search
    void searchPrevious();
    void searchNext();
    void anotherSearch();

    // Add torrent from browse local file
    //void downloadTorrent();

    // Add torrent from url
    void addTorrentFromUrl();



private:
    DBInterface    m_dbInterface;
    int            m_currentMovieID;

    const int MOVIE_TITLE_COL  = 0;
    const int MOVIE_YEAR_COL   = 1;
    const int MOVIE_GENRE_COL  = 2;
    const int MOVIE_RATING_COL = 3;
    const int MOVIE_ACTORS_COL = 4;
    const int MOVIE_ID_COL     = 5;
    const int MOVIE_IMAGE_COL = 6;

    const QString TREE_WIDTH_0 = "tree_width_0";
    const QString TREE_WIDTH_1 = "tree_width_1";
    const QString TREE_WIDTH_2 = "tree_width_2";
    const QString TREE_WIDTH_3 = "tree_width_3";
    const QString TREE_WIDTH_4 = "tree_width_4";

    const QString SPLITTER_POSITION = "splitter_position";
    const QString MAINWINDOW_GEOMETRY = "mainwindow_geometry";

    void search(bool searchDown);

    QTreeWidgetItem* getNextItem(QTreeWidgetItem* currentItem, bool searchDown);

    QLabel *m_title;
    QLabel *m_year;
    QLabel *m_genre;
    QLabel *m_rating;
    QLabel *m_actors;
    QString *imageName;
    QLineEdit *lineTitle;
    //QComboBox *comboGenre;
    //QSpinBox *spinboxYear;
    //QDoubleSpinBox *doublespinRating;
    QLineEdit *lineActors;
    QLineEdit *comboGenre;
    QLineEdit *spinboxYear;
    QLineEdit *doublespinRating;
    QTreeWidget *treeMovies;
    QPushButton *imageButton;
    QPushButton *downloadButton;
    QIcon *imageButtonIcon;
    QToolButton *btnSearchNext;
    QToolButton *btnSearchPrevious;
    QLabel *labelSearch;
    QComboBox *comboSearchField;
    QLineEdit *lineSearchTerm;
    QString fieldContents;
    QPushButton *searchButton;
};


#endif // MOVIESWINDOW_H

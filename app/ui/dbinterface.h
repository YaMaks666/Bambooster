#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include <QtSql>

struct MovieData
{
    int     id;
    QString genre;
    int     genreId;
    QString title;
    QString year;
    QString rating;
    QString actors;
    QString image;
};

class DBInterface
{
public:
    DBInterface();

    // Returns a QStringList containg contents of the genres table
    QStringList getGenres();

    // Returns all movie details for all movies in the movie table
    QList<MovieData> getMovies();

    // Update the contents of a movie
    void updateMovie(int movieId, const QString &title, int genreId, int year, double rating, const QString &actors);

    // Delete a movie
    void deleteMovie(int movieId);

    // Insert a movie
    int insertMovie(const QString& title, int genreID, int year, double rating);

private:
    QSqlDatabase m_database;


};

#endif // DBINTERFACE_H

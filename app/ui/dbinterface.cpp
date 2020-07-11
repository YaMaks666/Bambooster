#include "dbinterface.h"

DBInterface::DBInterface()
{
   m_database = QSqlDatabase::addDatabase("QSQLITE");
   QString applicationDir = QCoreApplication::applicationDirPath();
#if defined(WIN32)
   QString dbPath = QDir::cleanPath(applicationDir + "../../../movies.db");
#else
   QString dbPath = QDir::cleanPath(applicationDir + "../../../../../movies.db");
#endif

   qDebug() << "database path: " + dbPath;

   m_database.setDatabaseName(dbPath);

   if (!m_database.open())
   {
      qDebug() << "Error: connection failed";
   }
   else
   {
      qDebug() << "Database: connection ok";
   }
}

QStringList DBInterface::getGenres()
{
    QSqlQuery query("SELECT * FROM genre");
    int idName = query.record().indexOf("name");

    QStringList nameList;
    while (query.next())
    {
       QString name = query.value(idName).toString();
       nameList << name;
    }

    return nameList;
}

QList<MovieData> DBInterface::getMovies()
{
    QSqlQuery query("SELECT movie.id as id, genre.name as genre, genre_id, title, year, rating, actors, image "
                    "FROM movie "
                    "INNER JOIN genre ON movie.genre_id = genre.id");

    QList<MovieData> movieList;
    if (query.exec())
    {
        int idId = query.record().indexOf("id");
        int idGenre = query.record().indexOf("genre");
        int idGenreId = query.record().indexOf("genre_id");
        int idTitle = query.record().indexOf("title");
        int idYear = query.record().indexOf("year");
        int idRating = query.record().indexOf("rating");
        int idActors = query.record().indexOf("actors");
        int idImage = query.record().indexOf("image");
        while (query.next())
        {
           MovieData movieData;
           movieData.id = query.value(idId).toInt();
           movieData.genre = query.value(idGenre).toString();
           movieData.genreId = query.value(idGenreId).toInt();
           movieData.title = query.value(idTitle).toString();
           movieData.year = query.value(idYear).toString();
           movieData.rating = query.value(idRating).toString();
           movieData.actors = query.value(idActors).toString();
           movieData.image = query.value(idImage).toString();
           movieList << movieData;
        }
    }
    else
    {
        qDebug() << "Sql error in getMovies:" << query.lastError().text();
    }

    return movieList;
}

int DBInterface::insertMovie(const QString& title, int genreID, int year, double rating)
{
    QSqlQuery query;
    query.prepare("INSERT INTO movie(title, genre_id, year, rating) "
                    "values(:title, :genre_id, :year, :rating)");
    query.bindValue(":title", title);
    query.bindValue(":genre_id", genreID);
    query.bindValue(":year", year);
    query.bindValue(":rating", rating);
    if (!query.exec())
        qDebug() << query.lastError().text();

    return query.lastInsertId().toInt();
}

void DBInterface::updateMovie(int movieId, const QString &title, int genreId,
                              int year, double rating, const QString& actors)
{
    QSqlQuery query;
    query.prepare("UPDATE movie "
                  "SET "
                    "title = :title, "
                    "genre_id = :genreId, "
                    "year = :year, "
                    "rating = :rating, "
                    "actors = :actors "
                  "WHERE id = :movieID");
    query.bindValue(":title", title);
    query.bindValue(":genreId", genreId);
    query.bindValue(":year", year);
    query.bindValue(":rating", rating);
    query.bindValue(":actors", actors);
    query.bindValue(":movieID", movieId);
    if (!query.exec())
        qDebug() << query.lastError().text();
}

void DBInterface::deleteMovie(int movieId)
{
    // Delete the row in movie
    QSqlQuery movieQuery;
    movieQuery.prepare("DELETE FROM movie WHERE id = :movieId");
    movieQuery.bindValue(":movieId", movieId);
    if (!movieQuery.exec())
        qDebug() << movieQuery.lastError().text();

    // Also delete cast_members for this movie
    QSqlQuery castMemberQuery;
    castMemberQuery.prepare("DELETE FROM cast_member WHERE movie_id = :movieId");
    castMemberQuery.bindValue(":movieId", movieId);
    if (!castMemberQuery.exec())
        qDebug() << castMemberQuery.lastError().text();

}

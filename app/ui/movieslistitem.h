#ifndef MOVIESLISTITEM_H
#define MOVIESLISTITEM_H

#include <QTreeWidgetItem>
#include <QVariant>
#include <QMap>


class Movie;

class MoviesListItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT

public:
    enum Section {
        Title=0, Year, Genre, Rating, Actors
    };

    MoviesListItem(QTreeWidget *view, Movie *movie);

    void setSortData(int column, QVariant data);
    bool operator<(const QTreeWidgetItem &other) const;
    void refresh();
    // true if this torrent belongs to the currently opened section
    bool belongsToSection();

    Movie *movie() const;

    void setTitle(QString value);
    void setYear(int value);
    void setGenre(int genreId, QString value);
    void setRating(int value);
    void setActors(QString &value);


public slots:
//	void onOpenAction();
//	void onOpenLocationAction();
//	void onPauseAction();
//	void onStartAction();
//	void onStopAction();
//	void onRecheckAction();
//	void onRemoveAction();

signals:
    //void removeTorrent(Movie *movie, bool deleteData);

private:
    Movie *m_movie;
    QMap<int, QVariant> m_sortData;
};

#endif // MOVIESLISTITEM_H

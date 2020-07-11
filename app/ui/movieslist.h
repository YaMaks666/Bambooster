#ifndef MOVIESLIST_H
#define MOVIESLIST_H

#include "movieslistitem.h"
#include "panel.h"
#include <QTreeWidget>
#include <QList>

class Movie;

class MoviesList : public QTreeWidget
{
    Q_OBJECT

public:
    MoviesList();
    ~MoviesList();

    // Get item from list
    MoviesListItem *torrentItem(Movie *movie);
    MoviesListItem *torrentItem(const QString &name);
    Movie *currentTorrent();

public slots:
//	void addTorrent(Movie *movie);
//	void removeTorrent(Movie *movie);
//	void refresh();
//	void openContextMenu(const QPoint &pos);
//	void openHeaderContextMenu(const QPoint &pos);
//	void showAll();
//	void showCompleted();
//	void showDownloading();
//	void showUploading();

signals:
    //void removeTorrentSignal(Movie *movie, bool deleteData);

protected:
    // For the drag and drop functionality
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    QList<MoviesListItem *> m_items;
};




#endif // MOVIESLIST_H

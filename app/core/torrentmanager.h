#ifndef TORRENTMANAGER_H
#define TORRENTMANAGER_H

#include "torrentsettings.h"
#include <QObject>
#include <QList>
#include <QUrl>

class Torrent;
class TorrentInfo;

class TorrentManager : public QObject
{
	Q_OBJECT

public:
	TorrentManager();
	~TorrentManager();

	static TorrentManager* instance();

	/* Getters */
	const QList<Torrent *> &torrents() const;

signals:
	void torrentAdded(Torrent *torrent);
	void torrentRemoved(Torrent *torrent);
	// errors
	void failedToAddTorrent(QString errorString);
	void failedToResumeTorrents(QString errorString);
	void error(QString errorString);

public slots:
	// Loads all saved for resuming torrents
	void resumeTorrents();

	void addTorrentFromInfo(TorrentInfo *torrentInfo, const TorrentSettings &settings);

	// Saves resume info for all torrents
	void saveTorrentsResumeInfo();
	// Permanently saves the torrent file to the app data directory
	bool saveTorrentFile(const QString &filename, TorrentInfo *torrentInfo);

	bool removeTorrent(Torrent *torrent, bool deleteData);


private:
	QList<Torrent *> m_torrents;

	static TorrentManager *m_torrentManager;
};

#endif // TORRENTMANAGER_H

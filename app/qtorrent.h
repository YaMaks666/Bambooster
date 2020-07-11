#ifndef QTORRENT_H
#define QTORRENT_H

#include "core/torrentsettings.h"
#include <QObject>
#include <QHostAddress>
#include <QList>
#include <QString>
#include <QUrl>

class Torrent;
class TorrentManager;
class TorrentServer;
class MainWindow;
class LocalServiceDiscoveryClient;

class QTorrent : public QObject
{
	Q_OBJECT

public:
	QTorrent();
	~QTorrent();

	bool startServer();
	void startLSDClient();

	void shutDown();

	void showMainWindow();

	/* Opens a critical MessageBox */
	void critical(const QString &text);
	/* Opens an information MessageBox */
	void information(const QString &text);
	/* Opens question MessageBox. Returns true on 'yes' */
	bool question(const QString &text);
	/* Opens a warning MessageBox */
	void warning(const QString &text);

	const QByteArray &peerId() const;
	const QList<Torrent *> &torrents() const;
	TorrentManager *torrentManager();
	TorrentServer *server();
	MainWindow *mainWindow();

	static QTorrent *instance();

public slots:
	void LSDPeerFound(QHostAddress address, int port, Torrent *torrent);

private:
	QByteArray m_peerId;

	TorrentManager *m_torrentManager;
	TorrentServer *m_server;
	LocalServiceDiscoveryClient *m_LSDClient;

	MainWindow *m_mainWindow;

	static QTorrent *m_instance;
};

#endif // QTORRENT_H

#ifndef TRAFFICMONITOR_H
#define TRAFFICMONITOR_H

#include <QObject>
#include <QSet>
#include <QMap>
#include <QTimer>

class Peer;

class TrafficMonitor : public QObject
{
	Q_OBJECT

public:
	TrafficMonitor(QObject *parent = nullptr);

	qint64 uploadSpeed() const;
	qint64 downloadSpeed() const;

public slots:
	void addPeer(Peer *peer);
	void removePeer(Peer *peer);

	void onDataSent(qint64 bytes);
	void onDataReceived(qint64 bytes);

	void update();

signals:
	void uploadSpeedChanged(qint64 bytesPerSecond);
	void downloadSpeedChanged(qint64 bytesPerSecond);

private:
	qint64 m_uploadSpeed;
	qint64 m_downloadSpeed;
	QSet<Peer *> m_peers;
	QTimer m_timer;
	qint64 m_bytesUploaded;
	qint64 m_bytesDownloaded;
};

#endif // TRAFFICMONITOR_H

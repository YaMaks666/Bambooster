#include "trafficmonitor.h"
#include "peer.h"

#define MONITOR_INTERVAL 1000

TrafficMonitor::TrafficMonitor(QObject *parent)
	: QObject(parent)
	, m_uploadSpeed(0)
	, m_downloadSpeed(0)
	, m_bytesUploaded(0)
	, m_bytesDownloaded(0)
{
	m_timer.start(MONITOR_INTERVAL);
	connect(&m_timer, &QTimer::timeout, this, &TrafficMonitor::update);
}

qint64 TrafficMonitor::uploadSpeed() const
{
	return m_uploadSpeed;
}

qint64 TrafficMonitor::downloadSpeed() const
{
	return m_downloadSpeed;
}

void TrafficMonitor::addPeer(Peer *peer)
{
	connect(peer, &Peer::uploadedData, this, &TrafficMonitor::onDataSent);
	connect(peer, &Peer::downloadedData, this, &TrafficMonitor::onDataReceived);
}

void TrafficMonitor::removePeer(Peer *peer)
{
	disconnect(peer, &Peer::uploadedData, this, &TrafficMonitor::onDataSent);
	disconnect(peer, &Peer::downloadedData, this, &TrafficMonitor::onDataReceived);
}

void TrafficMonitor::onDataSent(qint64 bytes)
{
	m_bytesUploaded += bytes;
}

void TrafficMonitor::onDataReceived(qint64 bytes)
{
	m_bytesDownloaded += bytes;
}

void TrafficMonitor::update()
{
	m_uploadSpeed = m_bytesUploaded / (MONITOR_INTERVAL / 1000.0);
	m_downloadSpeed = m_bytesDownloaded / (MONITOR_INTERVAL / 1000.0);
	m_bytesUploaded = 0;
	m_bytesDownloaded = 0;
	emit uploadSpeedChanged(m_uploadSpeed);
	emit downloadSpeedChanged(m_downloadSpeed);
}

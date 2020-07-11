#include "torrentserver.h"
#include "peer.h"
#include <QSettings>
#include <QDebug>

TorrentServer::TorrentServer()
{
	connect(&m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

TorrentServer::~TorrentServer()
{
	disconnect(&m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

bool TorrentServer::startServer()
{
	if (m_server.isListening()) {
		m_server.close();
	}

	QSettings settings;
	quint64 startPort = settings.value("ServerStartPort", 6881).toInt();
	quint64 endPort = settings.value("ServerEndPort", 6889).toInt();
	settings.setValue("ServerStartPort", startPort);
	settings.setValue("ServerEndPort", endPort);

	if (endPort == 0)
		endPort = startPort;

	// Get a port in the range [startPort, endPort]
	if (startPort > 0) {
		for (quint16 port = startPort; port <= endPort; ++port) {
			if (m_server.listen(QHostAddress::Any, port)) {
				qDebug() << "Server started on port" << QString::number(port);
				return true;
			}
		}
	}

	// Just try to get a free port
	if (!m_server.listen()) {
		qDebug() << "Failed to start server:" << m_server.errorString();
		return false;
	}
	qDebug() << "Server started on port" << QString::number(m_server.serverPort());
	return true;
}

void TorrentServer::newConnection()
{
	QTcpSocket *socket = m_server.nextPendingConnection();
	Peer *peer = Peer::createClient(socket);
	m_peers.push_back(peer);
}

QTcpServer &TorrentServer::server()
{
	return m_server;
}

int TorrentServer::port()
{
	return m_server.serverPort();
}

QHostAddress TorrentServer::address()
{
	return m_server.serverAddress();
}

QList<Peer *> &TorrentServer::peers()
{
	return m_peers;
}

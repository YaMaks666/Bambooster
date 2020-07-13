#include "qtorrent.h"
#include "core/torrent.h"
#include "core/torrentinfo.h"
#include "core/torrentmanager.h"
#include "core/torrentserver.h"
#include "core/localservicediscoveryclient.h"
#include "core/trackerclient.h"
#include "ui/mainwindow.h"
#include <QGuiApplication>
#include <QMessageBox>
#include <QUrlQuery>

QTorrent *QTorrent::m_instance;

QTorrent::QTorrent()
{
	// Seed for the random number generator
	qsrand(QDateTime::currentMSecsSinceEpoch());

	m_instance = this;

	m_torrentManager = new TorrentManager;
	m_server = new TorrentServer;
	m_LSDClient = new LocalServiceDiscoveryClient;
	m_mainWindow = new MainWindow;

    // Generate random peer id that starts with 'B'
    m_peerId.push_back("B");
	while (m_peerId.size() < 20) {
		m_peerId.push_back(char(qrand() % 256));
	}

	connect(m_LSDClient, &LocalServiceDiscoveryClient::foundPeer, this, &QTorrent::LSDPeerFound);

	startServer();
	m_torrentManager->resumeTorrents();
	startLSDClient();
	showMainWindow();
}

QTorrent::~QTorrent()
{
	delete m_torrentManager;
	delete m_server;
	delete m_LSDClient;
	delete m_mainWindow;
}


bool QTorrent::startServer()
{
	return m_server->startServer();
}

void QTorrent::startLSDClient()
{
	m_LSDClient->announceAll();
	connect(m_torrentManager, &TorrentManager::torrentAdded, m_LSDClient, &LocalServiceDiscoveryClient::announceAll);
}

void QTorrent::shutDown()
{
	for (Torrent *torrent : torrents()) {
		torrent->stop();
	}
	m_torrentManager->saveTorrentsResumeInfo();
}

void QTorrent::showMainWindow()
{
	m_mainWindow->show();
}


/* Message Boxes */

void QTorrent::critical(const QString &text)
{
	QMessageBox::critical(m_mainWindow, QGuiApplication::applicationDisplayName(), text);
}

void QTorrent::information(const QString &text)
{
	QMessageBox::information(m_mainWindow, QGuiApplication::applicationDisplayName(), text);
}

bool QTorrent::question(const QString &text)
{
	QMessageBox::StandardButton ans;
	ans = QMessageBox::question(m_mainWindow, QGuiApplication::applicationDisplayName(), text);
	return ans == QMessageBox::Yes;
}

void QTorrent::warning(const QString &text)
{
	QMessageBox::warning(m_mainWindow, QGuiApplication::applicationDisplayName(), text);
}


const QByteArray &QTorrent::peerId() const
{
	return m_peerId;
}


const QList<Torrent *> &QTorrent::torrents() const
{
	return m_torrentManager->torrents();
}

TorrentManager *QTorrent::torrentManager()
{
	return m_torrentManager;
}

TorrentServer *QTorrent::server()
{
	return m_server;
}


MainWindow *QTorrent::mainWindow()
{
	return m_mainWindow;
}

QTorrent *QTorrent::instance()
{
	return m_instance;
}


void QTorrent::LSDPeerFound(QHostAddress address, int port, Torrent *torrent)
{
	torrent->connectToPeer(address, port);
}

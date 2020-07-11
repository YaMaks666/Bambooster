#include "filecontroller.h"
#include "torrent.h"
#include "torrentinfo.h"
#include "piece.h"
#include <QCryptographicHash>
#include <QThread>

FileController::FileController(Torrent *torrent)
	: m_torrent(torrent)
	, m_workerThread(new QThread)
{
	FileControllerWorker *worker = new FileControllerWorker(torrent);
	worker->moveToThread(m_workerThread);
	connect(m_workerThread, &QThread::finished, worker, &FileControllerWorker::deleteLater);

	m_workerThread->start();

	// For torrent-checking
	connect(this, &FileController::checkTorrent, worker, &FileControllerWorker::checkTorrent);
	connect(worker, &FileControllerWorker::torrentChecked, this, &FileController::torrentChecked);
	connect(worker, &FileControllerWorker::pieceAvailable, m_torrent, &Torrent::setPieceAvailable);
}

FileController::~FileController()
{
	m_workerThread->quit();
	m_workerThread->wait();
	delete m_workerThread;
}


FileControllerWorker::FileControllerWorker(Torrent *torrent)
	: m_torrent(torrent)
{
}

void FileControllerWorker::checkTorrent()
{
	TorrentInfo *info = m_torrent->torrentInfo();
	QList<Piece *> &pieces = m_torrent->pieces();
	for (Piece *piece : pieces) {
		emit pieceAvailable(piece, false);
	}
	for (Piece *piece : pieces) {
		QByteArray pieceData, pieceHash;
		if (!piece->getPieceData(pieceData)) {
			continue;
		}
		pieceHash = QCryptographicHash::hash(pieceData, QCryptographicHash::Sha1);
		bool pieceIsValid = (pieceHash == info->piece(piece->pieceNumber()));
		if (pieceIsValid) {
			emit pieceAvailable(piece, true);
		}
		// TODO report some kind of percentage
	}
	emit torrentChecked();
}

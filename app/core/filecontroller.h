#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>

class QThread;
class Torrent;
class Piece;

class FileControllerWorker : public QObject
{
	Q_OBJECT

public:
	FileControllerWorker(Torrent *torrent);

public slots:
	void checkTorrent();

signals:
	void torrentChecked();
	void pieceAvailable(Piece* piece, bool available);

private:
	Torrent *m_torrent;
};

class FileController : public QObject
{
	Q_OBJECT

public:
	FileController(Torrent *torrent);
	~FileController();

signals:
	void checkTorrent();
	void torrentChecked();

private:
	Torrent *m_torrent;
	QThread *m_workerThread;
};

#endif // FILECONTROLLER_H

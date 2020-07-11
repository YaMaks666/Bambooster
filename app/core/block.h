#ifndef BLOCK_H
#define BLOCK_H

#include <QObject>
#include <QByteArray>
#include <QList>

class Piece;
class Peer;

class Block : public QObject
{
	Q_OBJECT

private:
	Piece *m_piece;
	int m_begin;
	int m_size;
	bool m_isDownloaded;

	/* The peers to which this Block is
	 * assigned to be downloaded from. */
	QList<Peer *> m_assignees;

public:
	Block(Piece *piece, int begin, int size);
	~Block();
	Piece *piece();
	int begin() const;
	int size() const;
	bool isDownloaded();
	QList<Peer *> &assignees();
	bool hasAssignees() const;

signals:
	void downloaded(Block *block);

public slots:
	void setDownloaded(bool isDownloaded);
	void setData(const Peer *peer, const char *data);
	void addAssignee(Peer *peer);
	void removeAssignee(Peer *peer);
	void clearAssignees();
};

#endif // BLOCK_H

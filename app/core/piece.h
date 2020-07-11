#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QList>
#include <QByteArray>

class Torrent;
class Block;

class Piece : public QObject
{
	Q_OBJECT

private:
	Torrent *m_torrent;
	int m_pieceNumber;
	int m_size;
	bool m_isDownloaded;
	char *m_pieceData;
	QList<Block *> m_blocks;

	void addBlock(Block *block);
	bool checkIfFullyDownloaded();

public:
	Piece(Torrent *torrent, int pieceNumber, int size);
	~Piece();

	/* Getters */

	bool isDownloaded() const;
	int pieceNumber() const;
	char *data() const;
	int size() const;

	// Gets data for a block. Reads from files if needed
	bool getBlockData(int begin, int size, QByteArray &blockData);
	bool getPieceData(QByteArray &pieceData);
	// Returns a pointer to an existing block or nullptr if no such block exists
	Block *getBlock(int begin, int size) const;
	// Returns a block from this piece that hasn't been downloaded or requested
	Block *requestBlock(int size);

signals:
	void availabilityChanged(Piece *piece, bool isDownloaded);

public slots:
	// Update the block state: check if it's fully downloaded
	void updateState();
	void deleteBlock(Block *block);
	void unloadFromMemory();
	void setDownloaded(bool isDownloaded);
};

#endif // PIECE_H

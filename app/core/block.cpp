#include "block.h"
#include "piece.h"
#include "peer.h"

Block::Block(Piece *piece, int begin, int size)
	: m_piece(piece)
	, m_begin(begin)
	, m_size(size)
	, m_isDownloaded(false)
{
	connect(this, &Block::downloaded, m_piece, &Piece::updateState);
}

Block::~Block()
{
}

Piece *Block::piece()
{
	return m_piece;
}

int Block::begin() const
{
	return m_begin;
}

int Block::size() const
{
	return m_size;
}

bool Block::isDownloaded()
{
	return m_isDownloaded;
}

QList<Peer *> &Block::assignees()
{
	return m_assignees;
}

bool Block::hasAssignees() const
{
	return !m_assignees.isEmpty();
}

void Block::setDownloaded(bool isDownloaded)
{
	m_isDownloaded = isDownloaded;
	if (isDownloaded) {
		emit downloaded(this);
	}
}

void Block::setData(const Peer *peer, const char *data)
{
	if (isDownloaded()) {
		return;
	}

	char *p = m_piece->data() + m_begin;
	for (int i = 0; i < m_size; i++) {
		p[i] = data[i];
	}
	setDownloaded(true);
	QList<Peer *> assignees = m_assignees;
	for (auto p : assignees) {
		if (p != peer) {
			p->sendCancel(this);
		}
		p->releaseBlock(this);
	}
}


/* Assignee operations */

void Block::addAssignee(Peer *peer)
{
	m_assignees.push_back(peer);
}

void Block::removeAssignee(Peer *peer)
{
	for (int i = m_assignees.size() - 1; i >= 0; i--) {
		if (m_assignees[i] == peer) {
			m_assignees.removeAt(i);
		}
	}
}

void Block::clearAssignees()
{
	m_assignees.clear();
}

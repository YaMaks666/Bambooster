#ifndef TORRENTSTATUSBAR_H
#define TORRENTSTATUSBAR_H

#include <QTabWidget>

class QLabel;

class TorrentInfoPanel : public QTabWidget
{
	Q_OBJECT

public:
	TorrentInfoPanel(QWidget *parent = nullptr);

	void refreshInfoTab();

public slots:
	void refresh();
	void hide();

protected:
	void contextMenuEvent(QContextMenuEvent *event);

signals:
	void visibilityChanged(bool visible);

private:
	QLabel *m_torrentName;
	QLabel *m_torrentSize;
	QLabel *m_pieceSize;
	QLabel *m_infoHash;
	QLabel *m_creationDate;
	QLabel *m_createdBy;
	QLabel *m_comment;
};

#endif // TORRENTSTATUSBAR_H

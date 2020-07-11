#include "torrentinfopanel.h"
#include "qtorrent.h"
#include "mainwindow.h"
#include "torrentslist.h"
#include "core/torrent.h"
#include "core/torrentinfo.h"
#include "global.h"
#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QContextMenuEvent>
#include <QMenu>

TorrentInfoPanel::TorrentInfoPanel(QWidget *parent)
	: QTabWidget(parent)
{
	setFixedHeight(200);

	QWidget* infoTab = new QWidget;
	QVBoxLayout* infoLayout = new QVBoxLayout;
	infoLayout->addWidget(m_torrentName = new QLabel);
	infoLayout->addWidget(m_torrentSize = new QLabel);
	infoLayout->addWidget(m_pieceSize = new QLabel);
	infoLayout->addWidget(m_infoHash = new QLabel);
	infoLayout->addWidget(m_creationDate = new QLabel);
	infoLayout->addWidget(m_createdBy = new QLabel);
	infoLayout->addWidget(m_comment = new QLabel);
	infoTab->setLayout(infoLayout);

	addTab(infoTab, "Info");
    this->setStyleSheet("color: #414141;"
                        "background-color: #f4f4f4");
}

void TorrentInfoPanel::refreshInfoTab()
{
	Torrent *torrent = QTorrent::instance()->mainWindow()->
					   torrentsList()->currentTorrent();
	if (!torrent) {
		m_torrentName->clear();
		m_torrentSize->clear();
		m_pieceSize->clear();
		m_infoHash->clear();
		m_creationDate->clear();
		m_createdBy->clear();
		m_comment->clear();
		return;
	}
	TorrentInfo *info = torrent->torrentInfo();

	m_torrentName->setText(tr("Name: %1").arg(
							   QString::fromUtf8(info->torrentName())));
	m_torrentSize->setText(tr("Total Size: %1 (%2 bytes)")
						   .arg(formatSize(info->length()))
						   .arg(info->length()));
	m_pieceSize->setText(tr("Piece size: %1 (%2 bytes)")
						 .arg(formatSize(info->pieceLength()))
						 .arg(info->pieceLength()));
	m_infoHash->setText(tr("Info hash: %1").arg(
							QString::fromUtf8(info->infoHash().toHex())));
	m_creationDate->setText(tr("Creation date: %1")
							.arg(info->creationDate()
								 ? info->creationDate()->toString()
								 : tr("Not available")));
	m_createdBy->setText(tr("Created by: %1")
						 .arg(info->createdBy()
							  ? *info->createdBy()
							  : tr("Not available")));
	m_comment->setText(tr("Comment: %1")
					   .arg(info->comment()
							? *info->comment()
							: tr("Not available")));
}

void TorrentInfoPanel::refresh()
{
	refreshInfoTab();
}

void TorrentInfoPanel::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
	QAction *hide = new QAction(tr("Hide Info Panel"), &menu);
	menu.addAction(hide);
	connect(hide, &QAction::triggered, this, &TorrentInfoPanel::hide);
	menu.exec(event->globalPos());
}

void TorrentInfoPanel::hide()
{
	QWidget::hide();
	emit visibilityChanged(false);
}

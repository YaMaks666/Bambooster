#ifndef ADDTORRENTDIALOG_H
#define ADDTORRENTDIALOG_H

#include <QDialog>
#include <QUrl>
#include "core/torrentsettings.h"

class QLineEdit;
class QPushButton;
class QCheckBox;
class QLabel;
class TorrentInfo;

class AddTorrentDialog : public QDialog
{
	Q_OBJECT

public:
	AddTorrentDialog(QWidget *parent);
	~AddTorrentDialog();

	bool setTorrentUrl(QUrl url);
	bool browseFilePath(QWidget *parent);

	void updateInfo();

public slots:
	void browseDownloadLocation();
	void ok();
	void cancel();

signals:
	void torrentAdded(TorrentInfo* torrentInfo, TorrentSettings settings);

private:
	QLineEdit *m_downloadLocation;
	QPushButton *m_browseDownloadLocation;
	QCheckBox *m_startImmediately;
	QCheckBox *m_skipHashCheck;
	QPushButton *m_ok;
	QPushButton *m_cancel;

	QLabel *m_name;
	QLabel *m_size;
	QLabel *m_infoHash;
	QLabel *m_creationDate;
	QLabel *m_createdBy;
	QLabel *m_comment;

	TorrentInfo *m_torrentInfo;

	void connectAll();
	bool loadTorrent(const QString &filePath);
};

#endif // ADDTORRENTDIALOG_H

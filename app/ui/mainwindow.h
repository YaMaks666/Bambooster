#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QUrl>
#include <QGraphicsDropShadowEffect>

class Panel;
class QStackedWidget;
class TorrentsList;
class SettingsWindow;
class MoviesWindow;
class TorrentInfoPanel;
class Torrent;
class PlayerWindow;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();
	static MainWindow* instance();
	Panel *panel();
	TorrentsList *torrentsList();
    void closeEvent(QCloseEvent *event);
	QString getDownloadLocation();
    static MainWindow *m_mainWindow;

private:
	Panel *m_panel;
	QStackedWidget *m_stackedWidget;
	SettingsWindow *m_settingsWindow;
    MoviesWindow *m_moviesWindow;
	TorrentsList *m_torrentsList;
	TorrentInfoPanel *m_infoPanel;
    PlayerWindow *m_playerWindow;

	QAction *m_viewTorrentsFilterPanel;
	QAction *m_viewTorrentInfoPanel;

	QMenu *m_trayIconMenu;
	QSystemTrayIcon *m_trayIcon;

	QTimer m_refreshTimer;
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;



	// Creates all needed menus
	void createMenus();

public slots:
	void failedToAddTorrent(QString errorString);
	void failedToResumeTorrents(QString errorString);
	void showError(QString errorString);

	void openSettings();
	void closeSettings();

    void openMovies();
    void closeMovies();

    void openPlayer();
    void closePlayer();

	void addTorrentAction();
	void exitAction();

	void toggleHideShowTorrentsFilterPanel();
	void toggleHideShowTorrentInfoPanel();

	void aboutAction();

	void toggleHideShow();
	void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

	void torrentFullyDownloaded(Torrent *torrent);

	// Add torrent from url
	void addTorrentFromUrl(QUrl url);
};

#endif // MAINWINDOW_H

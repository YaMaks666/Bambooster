#ifndef PANEL_H
#define PANEL_H

#include <QToolBar>
#include <QIcon>
#include <QList>

class QToolButton;

class Panel : public QToolBar
{
	Q_OBJECT

public:
	enum Section {
        All, Completed, Downloading, Uploading, Settings, Movies, Player
	};

	Panel(QWidget *parent = nullptr);
	~Panel();
    Section getCurrentSection();

public slots:
	void openAll();
	void openCompleted();
	void openDownloading();
	void openUploading();
	void openSettings();
    void openMovies();
    void openPlayer();

signals:
	void showAll();
	void showCompleted();
	void showDownloading();
	void showUploading();
	void showSettings();
	void hideSettings();
    void showMovies();
    void hideMovies();
    void showPlayer();
    void hidePlayer();

private:
    void resetButtons();

	QIcon m_allIcon;
	QIcon m_completedIcon;
	QIcon m_downloadingIcon;
	QIcon m_uploadingIcon;
	QIcon m_settingsIcon;
    QIcon m_moviesIcon;
    QIcon m_playerIcon;

    QIcon m_allIconActive;
	QIcon m_completedIconActive;
	QIcon m_downloadingIconActive;
	QIcon m_uploadingIconActive;
	QIcon m_settingsIconActive;
    QIcon m_moviesIconActive;
    QIcon m_playerIconActive;

    QList<QToolButton *> m_toolButtons;

    QToolButton *m_all;
	QToolButton *m_completed;
	QToolButton *m_downloading;
	QToolButton *m_uploading;
	QToolButton *m_settings;
    QToolButton *m_movies;
    QToolButton *m_player;
};

#endif // PANEL_H

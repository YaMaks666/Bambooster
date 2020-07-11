#include "panel.h"
#include <QToolButton>

#define ICON_SIZE QSize(80, 60)
#define DEFAULT_COLOR "#414141"
#define CHECKED_COLOR "#262626"
#define HOVERED_COLOR "#505050"
#define HOVERED_CHECKED_COLOR "#303030"
#define PRESSED_COLOR "#323232"

Panel::Panel(QWidget *parent)
	: QToolBar("Torrents Filter Panel", parent)
	, m_allIcon(QIcon(":/button-all.png"))
	, m_completedIcon(QIcon(":/button-completed.png"))
	, m_downloadingIcon(QIcon(":/button-downloading.png"))
	, m_uploadingIcon(QIcon(":/button-uploading.png"))
	, m_settingsIcon(QIcon(":/button-settings.png"))
    , m_moviesIcon(QIcon(":/button-movies.png"))
    , m_playerIcon(QIcon(":/player.png"))

	, m_allIconActive(QIcon(":/button-all-active.png"))
	, m_completedIconActive(QIcon(":/button-completed-active.png"))
	, m_downloadingIconActive(QIcon(":/button-downloading-active.png"))
	, m_uploadingIconActive(QIcon(":/button-uploading-active.png"))
	, m_settingsIconActive(QIcon(":/button-settings-active.png"))
    , m_moviesIconActive(QIcon(":/button-movies-active.png"))
    , m_playerIconActive(QIcon(":/player-active.png"))

	, m_all(new QToolButton)
	, m_completed(new QToolButton)
	, m_downloading(new QToolButton)
	, m_uploading(new QToolButton)
	, m_settings(new QToolButton)
    , m_movies(new QToolButton)
    , m_player(new QToolButton)
{
	m_toolButtons.append(m_all);
	m_toolButtons.append(m_completed);
	m_toolButtons.append(m_downloading);
	m_toolButtons.append(m_uploading);
	m_toolButtons.append(m_settings);
    m_toolButtons.append(m_movies);
    m_toolButtons.append(m_player);

	resetButtons();

	for (auto button : m_toolButtons) {
		button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
		button->setCheckable(true);
		button->setAutoExclusive(true);
		addWidget(button);
	}

	setIconSize(ICON_SIZE);
	setStyleSheet("QToolBar { background: " DEFAULT_COLOR "; border: none; spacing: 0px; } "
				  "QToolButton { background-color: " DEFAULT_COLOR "; border: none } "
				  "QToolButton:checked { background-color: " CHECKED_COLOR " } "
				  "QToolButton:hover { background-color: " HOVERED_COLOR " } "
				  "QToolButton:hover:checked { background-color: " HOVERED_CHECKED_COLOR " } "
				  "QToolButton:pressed { background-color: " PRESSED_COLOR " }");

	setFloatable(false);

	connect(m_all, SIGNAL(clicked()), this, SLOT(openAll()));
	connect(m_completed, SIGNAL(clicked()), this, SLOT(openCompleted()));
	connect(m_downloading, SIGNAL(clicked()), this, SLOT(openDownloading()));
	connect(m_uploading, SIGNAL(clicked()), this, SLOT(openUploading()));
	connect(m_settings, SIGNAL(clicked()), this, SLOT(openSettings()));
    connect(m_movies, SIGNAL(clicked()), this, SLOT(openMovies()));
    connect(m_player, SIGNAL(clicked()), this, SLOT(openPlayer()));
}

Panel::~Panel()
{
	for (auto button : m_toolButtons) {
		delete button;
	}
}


Panel::Section Panel::getCurrentSection()
{
	if (m_all->isChecked()) {
		return Section::All;
	} else if (m_completed->isChecked()) {
		return Section::Completed;
	} else if (m_downloading->isChecked()) {
		return Section::Downloading;
	} else if (m_uploading->isChecked()) {
		return Section::Uploading;
	} else if (m_settings->isChecked()) {
		return Section::Settings;
    } else if (m_movies->isChecked()) {
        return Section::Movies;
    } else if (m_player->isChecked()) {
        return Section::Player;
    }
	Q_ASSERT(false);
	return Section::All;
}

void Panel::resetButtons()
{
	m_all->setIcon(m_allIcon);
	m_completed->setIcon(m_completedIcon);
	m_downloading->setIcon(m_downloadingIcon);
	m_uploading->setIcon(m_uploadingIcon);
	m_settings->setIcon(m_settingsIcon);
    m_movies->setIcon(m_moviesIcon);
    m_player->setIcon(m_playerIcon);
}

void Panel::openAll()
{
	resetButtons();
	m_all->setIcon(m_allIconActive);
	m_all->setChecked(true);
	emit showAll();
	emit hideSettings();
    emit hideMovies();
    emit hidePlayer();
}

void Panel::openCompleted()
{
	resetButtons();
	m_completed->setIcon(m_completedIconActive);
	m_completed->setChecked(true);
	emit showCompleted();
	emit hideSettings();
    emit hideMovies();
    emit hidePlayer();
}

void Panel::openDownloading()
{
	resetButtons();
	m_downloading->setIcon(m_downloadingIconActive);
	m_downloading->setChecked(true);
	emit showDownloading();
	emit hideSettings();
    emit hideMovies();
    emit hidePlayer();
}

void Panel::openUploading()
{
	resetButtons();
	m_uploading->setIcon(m_uploadingIconActive);
	m_uploading->setChecked(true);
	emit showUploading();
	emit hideSettings();
    emit hideMovies();
    emit hidePlayer();
}

void Panel::openSettings()
{
	resetButtons();
	m_settings->setIcon(m_settingsIconActive);
	m_settings->setChecked(true);
	emit showSettings();
    //emit hideMovies();
}

void Panel::openMovies()
{
    resetButtons();
    m_movies->setIcon(m_moviesIconActive);
    m_movies->setChecked(true);
    emit showMovies();
    //emit hideSettings();
}

void Panel::openPlayer()
{
    resetButtons();
    m_player->setIcon(m_playerIconActive);
    m_player->setChecked(true);
    emit showPlayer();
    //emit hideSettings();
}

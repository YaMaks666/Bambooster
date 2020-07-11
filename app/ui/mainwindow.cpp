#include "qtorrent.h"
#include "mainwindow.h"
#include "settingswindow.h"
#include "movieswindow.h"
#include "torrentslist.h"
#include "playerwindow.h"
#include "panel.h"
#include "torrentinfopanel.h"
#include "addtorrentdialog.h"
#include "core/torrent.h"
#include "core/torrentinfo.h"
#include "core/torrentmanager.h"
#include <QGuiApplication>
#include <QScreen>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QCloseEvent>
#include <QApplication>
#include <QVBoxLayout>


const int UI_REFRESH_INTERVAL = 300;

MainWindow *MainWindow::m_mainWindow = nullptr;

MainWindow::MainWindow()
{
    Q_ASSERT(m_mainWindow == nullptr);
	m_mainWindow = this;

	m_panel = new Panel;
	m_settingsWindow = new SettingsWindow;
    m_moviesWindow = new MoviesWindow;
	m_torrentsList = new TorrentsList;
	m_infoPanel = new TorrentInfoPanel;
    m_playerWindow = new PlayerWindow;

	connect(TorrentManager::instance(), &TorrentManager::torrentAdded,
			m_torrentsList, &TorrentsList::addTorrent);
	connect(TorrentManager::instance(), &TorrentManager::torrentRemoved,
			m_torrentsList, &TorrentsList::removeTorrent);
	connect(TorrentManager::instance(), &TorrentManager::failedToAddTorrent,
			this, &MainWindow::failedToAddTorrent);
	connect(TorrentManager::instance(), &TorrentManager::failedToResumeTorrents,
			this, &MainWindow::failedToResumeTorrents);
	connect(TorrentManager::instance(), &TorrentManager::error,
			this, &MainWindow::showError);

	connect(m_torrentsList, &TorrentsList::removeTorrentSignal,
			TorrentManager::instance(), &TorrentManager::removeTorrent);

	// Set the main window size to 3/4 of the screen size
	int width = QGuiApplication::primaryScreen()->size().width()*3/4;
	int height = QGuiApplication::primaryScreen()->size().height()*3/4;
	resize(width, height);
    setWindowIcon(QIcon(":/b.ico"));

	addToolBar(Qt::LeftToolBarArea, m_panel);

	m_stackedWidget = new QStackedWidget;
	QWidget *torrentsListWidget = new QWidget;
	QVBoxLayout *torrentsListLayout = new QVBoxLayout;
	torrentsListLayout->addWidget(m_torrentsList);
	torrentsListLayout->addWidget(m_infoPanel);
    torrentsListLayout->setContentsMargins(0, 0, 0, 0);
	torrentsListWidget->setLayout(torrentsListLayout);
	m_stackedWidget->addWidget(torrentsListWidget);
	m_stackedWidget->addWidget(m_settingsWindow);
    m_stackedWidget->addWidget(m_moviesWindow);
    m_stackedWidget->addWidget(m_playerWindow);
	setCentralWidget(m_stackedWidget);

	connect(m_panel, SIGNAL(showAll()), m_torrentsList, SLOT(showAll()));
	connect(m_panel, SIGNAL(showCompleted()), m_torrentsList, SLOT(showCompleted()));
	connect(m_panel, SIGNAL(showDownloading()), m_torrentsList, SLOT(showDownloading()));
	connect(m_panel, SIGNAL(showUploading()), m_torrentsList, SLOT(showUploading()));

	connect(m_panel, SIGNAL(showSettings()), this, SLOT(openSettings()));
	connect(m_panel, SIGNAL(hideSettings()), this, SLOT(closeSettings()));

    connect(m_panel, SIGNAL(showMovies()), this, SLOT(openMovies()));
    connect(m_panel, SIGNAL(hideMovies()), this, SLOT(closeMovies()));

    connect(m_panel, SIGNAL(showPlayer()), this, SLOT(openPlayer()));
    connect(m_panel, SIGNAL(hidePlayer()), this, SLOT(closePlayer()));

	m_panel->openAll();

	createMenus();

	m_refreshTimer.setInterval(UI_REFRESH_INTERVAL);
	m_refreshTimer.start();
	connect(&m_refreshTimer, SIGNAL(timeout()), m_torrentsList, SLOT(refresh()));
	connect(&m_refreshTimer, SIGNAL(timeout()), m_infoPanel, SLOT(refresh()));

	m_trayIconMenu = new QMenu(this);

    QAction *hideClientAction = new QAction(tr("Hide/Show Bambooster"), this);
	connect(hideClientAction, &QAction::triggered, this, &MainWindow::toggleHideShow);
	m_trayIconMenu->addAction(hideClientAction);

	QAction *exitAction = new QAction(tr("Exit"), this);
	connect(exitAction, &QAction::triggered, this, &MainWindow::exitAction);
	m_trayIconMenu->addAction(exitAction);

	m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/b.ico"));
	m_trayIcon->setContextMenu(m_trayIconMenu);

	m_trayIcon->show();



    //UI settings

    m_mainWindow->setStyleSheet("background-color: #414141;"
                                "color: #f4f4f4;"
                                "font-size: 14px;");

//    m_torrentsList->setStyleSheet("QTreeWidget::item:selected, QTreeWidget::item:selected:active, QTreeWidget::item:selected:!active {"
//                                      "border:none;"
//                                      "background-color: orange;"
//                                  "}");




	connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
}

MainWindow::~MainWindow()
{
}

MainWindow* MainWindow::instance()
{
	Q_ASSERT(m_mainWindow != nullptr);
	return m_mainWindow;
}

Panel *MainWindow::panel()
{
	return m_panel;
}

TorrentsList *MainWindow::torrentsList()
{
	return m_torrentsList;
}

void MainWindow::openSettings()
{
	m_stackedWidget->setCurrentIndex(1);
}

void MainWindow::closeSettings()
{
	m_stackedWidget->setCurrentIndex(0);
}

void MainWindow::openMovies()
{
    m_stackedWidget->setCurrentIndex(2);
}

void MainWindow::closeMovies()
{
    m_stackedWidget->setCurrentIndex(0);
}

void MainWindow::openPlayer()
{
    m_stackedWidget->setCurrentIndex(3);
}

void MainWindow::closePlayer()
{
    m_stackedWidget->setCurrentIndex(0);
}

void MainWindow::createMenus()
{
	menuBar()->show();

	// Menus
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    menuBar()->setStyleSheet("    background-color: orange;"
                             "    color: #414141");

	// Actions
	QAction *addTorrentAction = new QAction(tr("&Add torrent"), this);
	QAction *exitAction = new QAction(tr("&Exit"), this);
    QAction *hideClientAction = new QAction(tr("Hide Bambooster"), this);
    m_viewTorrentsFilterPanel = new QAction(tr("Side panel"), this);
	m_viewTorrentInfoPanel = new QAction(tr("Torrents info panel"), this);
	QAction *aboutAction = new QAction(tr("&About"), this);

	// Connect actions
	connect(addTorrentAction, &QAction::triggered, this, &MainWindow::addTorrentAction);
	connect(exitAction, &QAction::triggered, this, &MainWindow::exitAction);
	connect(hideClientAction, &QAction::triggered, this, &MainWindow::hide);
	connect(m_viewTorrentsFilterPanel, &QAction::triggered, this, &MainWindow::toggleHideShowTorrentsFilterPanel);
	connect(m_viewTorrentInfoPanel, &QAction::triggered, this, &MainWindow::toggleHideShowTorrentInfoPanel);
	connect(aboutAction, &QAction::triggered, this, &MainWindow::aboutAction);

	// Action shortcuts
	addTorrentAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	exitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));

	// Add actions to menus
	fileMenu->addAction(addTorrentAction);
	fileMenu->addAction(exitAction);

	viewMenu->addAction(hideClientAction);
	viewMenu->addSeparator();
	viewMenu->addAction(m_viewTorrentsFilterPanel);
	viewMenu->addAction(m_viewTorrentInfoPanel);

	helpMenu->addAction(aboutAction);

	// Configure actions
	m_viewTorrentsFilterPanel->setCheckable(true);
	m_viewTorrentInfoPanel->setCheckable(true);
	m_viewTorrentsFilterPanel->setChecked(true);
	m_viewTorrentInfoPanel->setChecked(true);
	connect(m_infoPanel, SIGNAL(visibilityChanged(bool)), m_viewTorrentInfoPanel, SLOT(setChecked(bool)));
	connect(m_panel, SIGNAL(visibilityChanged(bool)), m_viewTorrentsFilterPanel, SLOT(setChecked(bool)));
}


QString MainWindow::getDownloadLocation()
{
	// Open a dialog box to select the download directory
	QString downloadPath;
	downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
	downloadPath = QFileDialog::getExistingDirectory(this, tr("Select download directory"), downloadPath);
	// String is empty if user canceled the dialog box
	return downloadPath;
}

void MainWindow::failedToAddTorrent(QString errorString)
{
	QMessageBox::critical(this, tr("Failed to add torrent"), errorString);
}

void MainWindow::failedToResumeTorrents(QString errorString)
{
	QMessageBox::critical(this, tr("Failed to resume torrents"), "Failed to resume torrents\n\n" + errorString);
}

void MainWindow::showError(QString errorString)
{
	QMessageBox::critical(this, tr("Error"), errorString);
}

void MainWindow::addTorrentAction()
{
    AddTorrentDialog dialog(this);
    connect(&dialog, &AddTorrentDialog::torrentAdded,
            TorrentManager::instance(), &TorrentManager::addTorrentFromInfo);
    if (dialog.browseFilePath(this)) {
        dialog.exec();
    }
}

void MainWindow::exitAction()
{
	if (QTorrent::instance()->question("Are you sure you want to exit "
									   + QGuiApplication::applicationDisplayName() + "?")) {
		QApplication::quit();
	}
}

void MainWindow::addTorrentFromUrl(QUrl url)
{
    if (url.isValid()) {
        AddTorrentDialog dialog(this);
        connect(&dialog, &AddTorrentDialog::torrentAdded,
                TorrentManager::instance(), &TorrentManager::addTorrentFromInfo);
        if (dialog.setTorrentUrl(url)) {
            dialog.exec();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	event->ignore();
	hide();
}

void MainWindow::toggleHideShowTorrentsFilterPanel()
{
	if (m_panel->isHidden()) {
		m_panel->show();
	} else {
		m_panel->hide();
	}
}

void MainWindow::toggleHideShowTorrentInfoPanel()
{
	if (m_infoPanel->isHidden()) {
		m_infoPanel->show();
	} else {
		m_infoPanel->hide();
	}
}

void MainWindow::toggleHideShow()
{
	if (isHidden()) {
		show();
	} else {
		hide();
	}
}

void MainWindow::aboutAction()
{
    QMessageBox::about(this, tr("About Bambooster"),
                       tr("<p><b>Bambooster</b> is a simple BitTorrent client, written from "
                          "scratch in C++ with Qt5. Bambooster aims to be a good, lightweight "
						  "alternative to all the other BitTorrent clients.</p>"
                          "<p>You are currently using <b>version " VERSION "</b></p>"));
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick) {
		show();
	}
}

void MainWindow::torrentFullyDownloaded(Torrent *torrent)
{
	m_trayIcon->showMessage(tr("Torrent downloaded successfully"),
							tr("The torrent %1 was successfully downloaded")
							.arg(QString::fromUtf8(torrent->torrentInfo()->torrentName())));
}

#include "playlistwindow.h"
#include "ui_playlistwindow.h"

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStandardItemModel>
#include <QList>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTextStream>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QCloseEvent>
#ifndef DEBUG
#include <QDebug>
#endif

PlaylistWindow::PlaylistWindow(QMediaPlayer *player, QWidget *parent) :
    QWidget(parent), ui(new Ui::PlaylistWindow),
    m_player{player}
{
    ui->setupUi(this);
    setAcceptDrops(true);
    this->setWindowTitle("PlayList");
    m_playlist_model = new QStandardItemModel(this);
    ui->plsView->setModel(m_playlist_model);
    m_playlist = new QMediaPlaylist(this);
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    m_player->setPlaylist(m_playlist);
    configure_playlist_view();
    ui->plsView->setContextMenuPolicy(Qt::ContextMenuPolicy::ActionsContextMenu);
    ui->plsView->addAction(ui->actionAdd_video_to_playlist);
    ui->plsView->addAction(ui->action_remove_selected_items_from_playlist);
    ui->plsView->addAction(ui->action_clear_playlist);
    ui->plsView->addAction(ui->action_remove_selected_items_from_HDD);

    //Connections

    connect(ui->plsView, &QTableView::doubleClicked,
            [this](const QModelIndex &index)
    {
        m_playlist->setCurrentIndex(index.row());
        ui->plsView->clearSelection();
        ui->plsView->selectRow(index.row());
        if(m_player->state() != QMediaPlayer::PlayingState)
            m_player->play();
    });

    connect(m_playlist, &QMediaPlaylist::currentIndexChanged,
            [this](int row)
    {
        auto title = m_playlist_model->data(m_playlist_model->index(row, 0))
                .toString();
        emit current_movie_title_changed(title);
    });

    connect(ui->btnLoadPlaylist, &QPushButton::clicked, this,
            &PlaylistWindow::load_choosen_playlist);
    connect(ui->btnSavePlaylist, &QPushButton::clicked, this,
            &PlaylistWindow::save_playlist_as);
    connect(ui->btnClearPlaylist, &QPushButton::clicked, this,
            &PlaylistWindow::clear_playlist);
    connect(ui->btnRemoveSelected, &QPushButton::clicked, this,
            &PlaylistWindow::remove_selected_items_from_playlist);

    QFile file(mLastPlaylistFilePath);
    if(file.exists())
    {
        load_playlist_from_file(mLastPlaylistFilePath);
    }
}

PlaylistWindow::~PlaylistWindow()
{
    save_playlist_to_file(mLastPlaylistFilePath);
    delete m_playlist;
    delete m_playlist_model;
    delete ui;
}

void PlaylistWindow::add_to_playlist()
{
    auto dir = QStandardPaths::standardLocations(QStandardPaths::MoviesLocation)
            .value(0, QDir::homePath());

    QStringList selectedFiles = QFileDialog::getOpenFileNames(this, "Open a tunes", dir,
                                                      "Video files (*.mp4 *.avi *\
                                                      *.flv *.mpeg *.mpg *.3gp);;\
                                                      Audio files (*.mp3 *.ogg *\
                                                      *.wav);;All files (*.*)");
    QStringList extensions = {"mp4", "avi", "flv", "mpeg", "mpg", "3gp", "mp3",
                            "wav", "ogg"};
    QStringList videos;
    for(auto path: selectedFiles)
    {
        QFileInfo fileInfo(path);
        if(!fileInfo.isDir() && extensions.contains(fileInfo.suffix()))
            videos << path;
    }

    add_to_playlist(videos);
}

void PlaylistWindow::add_to_playlist(QStringList &videos)
{
    for(const auto &video: videos)
    {
        QList<QStandardItem*> items;
        items.append(new QStandardItem(QDir(video).dirName()));
        items.append(new QStandardItem(video));
        m_playlist_model->appendRow(items);
        m_playlist->addMedia(QUrl::fromLocalFile(video));
    }
    configure_playlist_view();
    if(m_player->state() != QMediaPlayer::PlayingState)
        m_player->play();
}

void PlaylistWindow::prev_movie()
{
    m_playlist->previous();
    auto index = m_playlist->currentIndex();
    ui->plsView->clearSelection();
    ui->plsView->selectRow(index);
}

void PlaylistWindow::next_movie()
{
    m_playlist->next();
    auto index = m_playlist->currentIndex();
    ui->plsView->clearSelection();
    ui->plsView->selectRow(index);
}

void PlaylistWindow::add_files_from_mime_data(const QMimeData *mimeData)
{
    if(!mimeData) return;
    QStringList extensions = {"mp4", "avi", "flv", "mpeg", "mpg", "3gp", "mp3",
                              "wav", "ogg"};
    auto urls = mimeData->urls();
    QStringList videoFiles;
    for(auto url: urls)
    {
        if(url.isLocalFile())
        {
            auto file = url.toLocalFile();
            QFileInfo info(file);
            if(!info.isDir() && extensions.contains(info.suffix()))
                videoFiles << url.toLocalFile();
        }
    }
    add_to_playlist(videoFiles);
}

int PlaylistWindow::getNumberOfItemsInPlaylist() const
{
    return m_playlist_model->rowCount();
}

int PlaylistWindow::getCurrentItemIndex() const
{
    return m_playlist->currentIndex();
}

void PlaylistWindow::setCurrentItem(int index)
{
    m_playlist->setCurrentIndex(index);
    m_current_index = index;
    ui->plsView->selectRow(m_current_index);
}

void PlaylistWindow::closeEvent(QCloseEvent*)
{
    this->hide();
}

void PlaylistWindow::keyReleaseEvent(QKeyEvent *event)
{
    auto key = event->key();
    if(key == Qt::Key_Return)
    {
        auto selectedRows = ui->plsView->selectionModel()->selectedRows();
        if(selectedRows.size() == 1)
        {
            m_playlist->setCurrentIndex(selectedRows.at(0).row());
            if(m_player->state() != QMediaPlayer::PlayingState)
            {
                m_player->play();
            }
        }
    }
    else if(key == Qt::Key_Delete)
    {
        remove_selected_items_from_playlist();
    }
    else if(key == Qt::Key_A && event->modifiers() & Qt::Key_Control)
    {
        ui->plsView->selectAll();
    }
    else if(key == Qt::Key_Escape)
    {
        ui->plsView->clearSelection();
    }
    else if(key == Qt::Key_Up)
    {
        if(m_playlist_model->rowCount() > 0)
        {
            --m_current_index;
            if(m_current_index < 0)
                m_current_index = m_playlist_model->rowCount() - 1;
            ui->plsView->clearSelection();
            ui->plsView->selectRow(m_current_index);
        }
    }
    else if(key == Qt::Key_Down)
    {
        if(m_playlist_model->rowCount() > 0)
        {
            ++m_current_index;
            if(m_current_index > m_playlist_model->rowCount() - 1)
                m_current_index = 0;
            ui->plsView->clearSelection();
            ui->plsView->selectRow(m_current_index);
        }
    }
    else
    {
        QWidget::keyReleaseEvent(event);
    }
}

void PlaylistWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void PlaylistWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void PlaylistWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void PlaylistWindow::dropEvent(QDropEvent *event)
{
    QStringList extensions = {"mp4", "avi", "flv", "mpeg", "mpg", "3gp"};
    auto data = event->mimeData();
    auto urls = data->urls();
    QStringList videoFiles;
    for(auto url: urls)
    {
        if(url.isLocalFile())
        {
            auto file = url.toLocalFile();
            QFileInfo info(file);
            if(!info.isDir() && extensions.contains(info.suffix()))
                videoFiles << url.toLocalFile();
        }
    }
    add_to_playlist(videoFiles);
}

void PlaylistWindow::on_btnHide_clicked()
{
    this->hide();
}

void PlaylistWindow::load_choosen_playlist()
{
    QString startLocation = QStandardPaths::standardLocations(
                QStandardPaths::MoviesLocation).value(0, QDir::homePath());
    QString filter = QString::fromStdString("Playlists (*.tpls)");
    QString fileName = QFileDialog::getOpenFileName(
                this,
                QString::fromStdString("Open playlist"),
                startLocation,
                filter);
    load_playlist_from_file(fileName);
}

void PlaylistWindow::save_playlist_as()
{
    QString startLocation = QStandardPaths::standardLocations(
                QStandardPaths::MoviesLocation).value(0, QDir::homePath());
    QString fileName = QFileDialog::getSaveFileName(
                this,
                QString::fromStdString("Save playlist to file..."),
                startLocation,
                QString::fromStdString("Playlists (*.tpls)"));
    if(!fileName.contains(".tpls"))
        fileName += QString(".tpls");
    save_playlist_to_file(fileName);
}

void PlaylistWindow::clear_playlist()
{
    m_playlist->clear();
    m_playlist_model->clear();
}

void PlaylistWindow::remove_selected_items_from_playlist()
{
    auto itemsToDelete = ui->plsView->selectionModel()->selectedRows();
    while(!itemsToDelete.isEmpty())
    {
        m_playlist_model->removeRows(itemsToDelete.last().row(), 1);
        itemsToDelete.removeLast();
    }
}

void PlaylistWindow::remove_selected_items_from_hdd()
{
    QMessageBox::StandardButton reply =
            QMessageBox::question(this, "Confirm deleting",
                                  "Do you really want to delete\
                                  selected movies from your HDD?",
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        auto itemsToDelete = ui->plsView->selectionModel()->selectedRows();
        while(!itemsToDelete.isEmpty())
        {
            auto row = itemsToDelete.last().row();
            m_playlist->removeMedia(row);
            QString path = m_playlist_model->item(row, 1)->text();
            m_playlist_model->removeRows(row, 1);
            itemsToDelete.removeLast();
            QFile file(path);
            if(file.exists())
            {
                if(!file.remove())
                {
                    QMessageBox::critical(this, "Error", "Failed to remove");
                }
            }
        }
    }
}

bool PlaylistWindow::load_playlist_from_file(const QString &filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Error", file.errorString());
        return false;
    }
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString videoName = stream.readLine();
        QString videoPath = stream.readLine();
        QList<QStandardItem*> list;
        list.append(new QStandardItem(videoName));
        list.append(new QStandardItem(videoPath));
        m_playlist_model->appendRow(list);
        m_playlist->addMedia(QUrl::fromLocalFile(videoPath));
    }
    file.close();
    configure_playlist_view();
    return true;
}

bool PlaylistWindow::save_playlist_to_file(const QString &filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "Error", file.errorString());
        return false;
    }
    QTextStream stream(&file);
    for(int row{0}; row < m_playlist_model->rowCount(); ++row)
    {
        stream << m_playlist_model->item(row, 0)->text() << "\n" <<
                m_playlist_model->item(row, 1)->text() << endl;
    }
    file.close();
    return true;
}

void PlaylistWindow::configure_playlist_view()
{
    QStringList headers;
    headers << tr("Tune") << tr("File path");
    m_playlist_model->setHorizontalHeaderLabels(headers);
    ui->plsView->hideColumn(1); // Hide file path
    ui->plsView->verticalHeader()->setVisible(false); //Hide column numbers
    ui->plsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->plsView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->plsView->resizeColumnsToContents();
    ui->plsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->plsView->horizontalHeader()->setStretchLastSection(true);
    ui->plsView->setColumnWidth(1, ui->plsView->width());
}

void PlaylistWindow::on_action_remove_selected_items_from_playlist_triggered()
{
    this->remove_selected_items_from_playlist();
}

void PlaylistWindow::on_action_remove_selected_items_from_HDD_triggered()
{
    this->remove_selected_items_from_hdd();
}

void PlaylistWindow::on_action_clear_playlist_triggered()
{
    this->clear_playlist();
}

void PlaylistWindow::on_actionAdd_video_to_playlist_triggered()
{
    this->add_to_playlist();
}

void PlaylistWindow::on_btnAddVideo_clicked()
{
    this->add_to_playlist();
}

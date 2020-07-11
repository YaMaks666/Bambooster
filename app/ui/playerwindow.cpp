#include "playerwindow.h"
#include "videowidget.h"
#include "playlistwindow.h"
#include "core/settingsutil.h"
#include "ui_playlistwindow.h"
#include <QProgressBar>
#include <QSlider>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QKeyEvent>
#include <QStringList>
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIntValidator>
#include <QSettings>
#include <QMessageBox>
#include <QGuiApplication>
#include <QProgressBar>
#include <QSlider>
#include <QMouseEvent>
#include <sstream>

PlayerWindow::PlayerWindow(QWidget *parent)
    : QWidget(parent)
    , actionOpenIcon(QIcon(":/btnOpen.png"))
    , actionPlayIcon(QIcon(":/btnPlay.png"))
    , actionPauseIcon(QIcon(":/btnPause.png"))
    , actionStopIcon(QIcon(":/btnStop.png"))
    , actionSeekBackwardIcon(QIcon(":/btnSeekBackward.png"))
    , actionSeekForwardIcon(QIcon(":/btnSeekForward.png"))
    , actionPrevIcon(QIcon(":/btnPrev.png"))
    , actionNextIcon(QIcon(":/btnNext.png"))
    , actionPlaylistIcon(QIcon(":/btnPlaylist.png"))

{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QHBoxLayout *controlPanelLayout = new QHBoxLayout;

    controlPanelLayout->addWidget(mainToolBar = new QToolBar);
    controlPanelLayout->addWidget(statusBar = new QStatusBar);

    mainLayout->addWidget(m_video_widget = new VideoWidget);
    mainLayout->addLayout(controlPanelLayout);
    mainToolBar->addAction(actionOpen = new QAction);
    mainToolBar->addAction(actionPlay = new QAction);
    mainToolBar->addAction(actionPause = new QAction);
    mainToolBar->addAction(actionStop = new QAction);
    mainToolBar->addAction(action_prev = new QAction);
    mainToolBar->addAction(action_next = new QAction);
    mainToolBar->addAction(action_show_playlist = new QAction);
    mainToolBar->addAction(action_seek_backwards = new QAction);
    mainToolBar->addAction(action_seek_forwards = new QAction);
    mainToolBar->setMinimumWidth(320);


    mainLayout->setContentsMargins(0, 0, 0, 0);
    statusBar->setStyleSheet("background-color: rgb(244, 244, 244);"
                             "color: #414141;");
    mainToolBar->setStyleSheet("background-color: rgb(244, 244, 244);"
                               "color: #414141;");
    setStyleSheet("QSlider::groove:horizontal {"
                  "border: 1px solid #999999;"
                  "height: 8px;"
                  "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                  "margin: 2px 0;"
                  "}"
                  "QSlider::handle:horizontal {"
                  "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                  "border: 1px solid #5c5c5c;"
                  "width: 18px;"
                  "margin: -2px 0;"
                  "border-radius: 3px;"
                  "}");
    mainLayout->setSpacing(0);
    controlPanelLayout->setSpacing(0);
    controlPanelLayout->setAlignment(Qt::AlignBottom);
    mainLayout->setStretchFactor(m_video_widget, 10);
    //controlPanelLayout->setStretchFactor(mainToolBar, 1);
    //controlPanelLayout->setStretchFactor(m_seek_slider, 5);
    //statusBar->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    actionOpen->setIcon(actionOpenIcon);
    actionPlay->setIcon(actionPlayIcon);
    actionPause->setIcon(actionPauseIcon);
    actionStop->setIcon(actionStopIcon);
    action_show_playlist->setIcon(actionPlaylistIcon);
    action_next->setIcon(actionNextIcon);
    action_prev->setIcon(actionPrevIcon);
    action_seek_forwards->setIcon(actionSeekForwardIcon);
    action_seek_backwards->setIcon(actionSeekBackwardIcon);

    m_player = new QMediaPlayer(this);
    m_player->setVideoOutput(m_video_widget);
    m_player->setVolume(50);
    m_pls_window = new PlaylistWindow(m_player);
    m_pls_window->hide();
    m_current_play_time_label = new QLabel(this);
    m_seek_bar = new QProgressBar(this);
    m_seek_slider = new QSlider(this);
    m_seek_slider->setOrientation(Qt::Horizontal);
    m_seek_slider->setMaximum(100);
    m_seek_slider->setMaximumWidth(1000);
    m_remaining_time_label = new QLabel(this);

    m_volume_label = new QLabel("Volume:50%",this);
    m_volume_slider = new QSlider(this);
    m_volume_slider->setOrientation(Qt::Horizontal);
    m_volume_slider->setMaximumWidth(70);
    m_volume_slider->setMaximum(100);
    m_volume_slider->setMinimum(0);
    m_volume_slider->setValue(50);
    m_video_widget->setStyleSheet("background-color: black;");
    m_video_widget->setContentsMargins(0, 0, 0, 0);

    statusBar->addWidget(m_seek_bar);    
    statusBar->addWidget(m_seek_slider);
    statusBar->addWidget(m_current_play_time_label);
    statusBar->addWidget(m_remaining_time_label);
    statusBar->addWidget(m_volume_label);
    statusBar->addWidget(m_volume_slider);
    m_seek_bar->hide();

    //connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &PlayerWindow::statusChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this,
            &PlayerWindow::update_seek_slider_value);
    connect(m_player, &QMediaPlayer::positionChanged, this,
            &PlayerWindow::update_time_labels);
    connect(m_seek_slider, &QSlider::sliderMoved, this,
            &PlayerWindow::update_player_pos);
    //connect(m_seek_slider, &QSlider::valueChanged, this, &PlayerWindow::update_player_pos);
    //connect(m_seek_slider, &QSlider::sliderPressed, this, &PlayerWindow::update_player_pos2);
    connect(m_player, &QMediaPlayer::durationChanged, m_seek_bar,
            &QProgressBar::setMaximum);
    connect(m_player, &QMediaPlayer::positionChanged, m_seek_bar,
            &QProgressBar::setValue);
    connect(m_video_widget, &VideoWidget::pauseRequestInFullscreenMode,
            this, &PlayerWindow::on_actionPause_triggered);
    connect(m_player, &QMediaPlayer::volumeChanged, m_volume_slider,
            &QSlider::setValue);
    connect(m_volume_slider, &QSlider::sliderMoved, m_player,
            &QMediaPlayer::setVolume);
    connect(m_volume_slider, &QSlider::valueChanged, m_player,
            &QMediaPlayer::setVolume);
    connect(m_volume_slider, &QSlider::valueChanged, this,
            &PlayerWindow::update_volume_label);
    connect(m_video_widget, &VideoWidget::volumeLevelChanged, this,
            &PlayerWindow::change_volume_level);

    connect(m_pls_window, &PlaylistWindow::current_movie_title_changed, this, &PlayerWindow::update_title);

    connect(actionOpen, &QAction::triggered, this, &PlayerWindow::on_actionOpen_triggered);
    connect(actionPlay, &QAction::triggered, this, &PlayerWindow::on_actionPlay_triggered);
    connect(actionPause, &QAction::triggered, this, &PlayerWindow::on_actionPause_triggered);
    connect(actionStop, &QAction::triggered, this, &PlayerWindow::on_actionStop_triggered);
    connect(action_next, &QAction::triggered, this, &PlayerWindow::on_action_next_triggered);
    connect(action_prev, &QAction::triggered, this, &PlayerWindow::on_action_prev_triggered);
    connect(action_show_playlist, &QAction::triggered, this, &PlayerWindow::on_action_show_playlist_triggered);
    connect(action_seek_forwards, &QAction::triggered, this, &PlayerWindow::on_action_seek_forwards_triggered);
    connect(action_seek_backwards, &QAction::triggered, this, &PlayerWindow::on_action_seek_backwards_triggered);

    load_settings();
}

PlayerWindow::~PlayerWindow()
{
    save_settings();
    delete m_pls_window;
    //delete ui;
}

void PlayerWindow::on_actionOpen_triggered()
{
    m_pls_window->add_to_playlist();
}

void PlayerWindow::on_actionPlay_triggered()
{
    m_player->play();
    //statusBar->showMessage("Playing ...");
}

void PlayerWindow::on_actionPause_triggered()
{
    if(m_player->state() == QMediaPlayer::State::PlayingState)
    {
        m_player->pause();
        //statusBar->showMessage("Paused");
    }
    else if(m_player->state() == QMediaPlayer::State::PausedState)
    {
        m_player->play();
        //statusBar->showMessage("Playing ...");
    }
}

void PlayerWindow::on_actionStop_triggered()
{
    m_player->stop();
    //statusBar->showMessage("Stop");
}

void PlayerWindow::update_volume_label(int curr_vol)
{
    m_volume_label->setText(QString("Volume:") + QString::number(curr_vol) +
                            QString("%"));
}

void PlayerWindow::change_volume_level(int delta)
{
    if(delta > 0)
        m_volume_slider->setValue(m_volume_slider->value() + 1);
    else if(delta < 0)
        m_volume_slider->setValue(m_volume_slider->value() - 1);
}

void PlayerWindow::update_seek_slider_value(qint64 curr_player_pos)
{
     m_seek_slider->setValue((int)((float)curr_player_pos /
                                   m_player->duration() * 100));
}

void PlayerWindow::update_player_pos(int seek_slider_pos)
{
    auto p = m_player->duration() * seek_slider_pos / 100;
    m_player->setPosition(p);
}

void PlayerWindow::update_time_labels(int curr_player_pos)
{
    auto curr_pos = curr_player_pos / 1000;
    std::stringstream ss_curr_pos;
    ss_curr_pos << curr_pos / 3600 << ":" << (curr_pos % 3600) / 60 << ":" <<
        (curr_pos % 3600) % 60 ;
    auto total_duration = m_player->duration() / 1000;
    std::stringstream ss_total_dur;
    ss_total_dur << total_duration / 3600 << ":" <<
                    (total_duration % 3600) / 60  << ":" <<
                    (total_duration % 3600) % 60 ;
    auto remaining_time = total_duration - curr_pos;
    std::stringstream ss_remains;
    ss_remains << remaining_time / 3600 << ":" <<
                  (remaining_time % 3600) / 60 << ":" <<
                  (remaining_time % 3600) % 60 ;
    m_current_play_time_label->setText(QString::fromStdString
                                       (ss_curr_pos.str()));
    m_remaining_time_label->setText(QString("%1/(%2)")
                                    .arg(QString::fromStdString(
                                             ss_remains.str()))
                                    .arg(QString::fromStdString(
                                             ss_total_dur.str())));
}

void PlayerWindow::update_title(QString newTitle)
{
    this->setWindowTitle("TeMedia [" + newTitle + "]");
}


void PlayerWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::MiddleButton)
    {
        on_actionPause_triggered();
    }
    else
    {
        PlayerWindow::mouseReleaseEvent(event);
    }
}

void PlayerWindow::keyReleaseEvent(QKeyEvent *event)
{
    auto key = event->key();
    if(key == Qt::Key_Space){
        on_actionPause_triggered();
    }
    else if(key == Qt::Key_Left)
    {
        on_action_seek_backwards_triggered();
    }
    else if(key == Qt::Key_Right)
    {
        on_action_seek_forwards_triggered();
    }
    else if(key== Qt::Key_F && !this->isFullScreen())
    {
        m_video_widget->toggleFullScreenMode();
        event->accept();
    }
}

void PlayerWindow::wheelEvent(QWheelEvent *event)
{
    change_volume_level(event->angleDelta().y());
}

void PlayerWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void PlayerWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void PlayerWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void PlayerWindow::dropEvent(QDropEvent *event)
{
    m_pls_window->add_files_from_mime_data(event->mimeData());
}


void PlayerWindow::displayErrorMessage(const QString &msg)
{
    QMessageBox box;
    box.setText(msg);
    box.exec();
}

/*void PlayerWindow::resizeEvent(QResizeEvent *event)
{
    qDebug() << "w = " << this->width() << " h = " << this->height();
    QPlayerWindow::resizeEvent(event);
}*/

void PlayerWindow::on_action_seek_forwards_triggered()
{
    m_player->setPosition(m_player->position() + 5000);
}

void PlayerWindow::on_action_seek_backwards_triggered()
{
    m_player->setPosition(m_player->position() - 5000);
}

void PlayerWindow::on_action_show_playlist_triggered()
{
    if(m_pls_window->isHidden())
        m_pls_window->show();
    else
        m_pls_window->hide();
}

void PlayerWindow::on_action_prev_triggered()
{
    m_pls_window->prev_movie();
}

void PlayerWindow::on_action_next_triggered()
{
    m_pls_window->next_movie();
}

void PlayerWindow::save_settings()
{
    saveParameter(SKEY_WINDOW_POS, pos(), SGROUP_WINDOW_GEOMETRY);
    saveParameter(SKEY_WINDOW_SIZE, size(), SGROUP_WINDOW_GEOMETRY);
    saveParameter(SKEY_CURR_PLAYLIST_ITEM, m_pls_window->getCurrentItemIndex(), SGROUP_PLAYLIST);
    saveParameter(SKEY_PLAYER_POS, m_player->position(), SGROUP_PLAYER);
    saveParameter(SKEY_IS_PLAYER_PAUSED,
                  m_player->state() == QMediaPlayer::State::PausedState,
                  SGROUP_PLAYER);
    saveParameter(SKEY_VOLUME_VALUE, m_volume_slider->value(), SGROUP_PLAYER);
}

void PlayerWindow::load_settings()
{
    auto restoredPos = loadParameter(SKEY_WINDOW_POS, SGROUP_WINDOW_GEOMETRY,
                                     pos()).value<QPoint>();
    this->move(restoredPos);

    auto restoredSize = loadParameter(SKEY_WINDOW_SIZE, SGROUP_WINDOW_GEOMETRY,
                                      size()).value<QSize>();
    this->resize(restoredSize);

    int currPlaylistItemIndex = loadParameter(SKEY_CURR_PLAYLIST_ITEM,
                                              SGROUP_PLAYLIST, 0).toInt();
    this->m_pls_window->setCurrentItem(currPlaylistItemIndex);
    this->m_pls_window->setCurrentItem(currPlaylistItemIndex);

    int volumeValue = loadParameter(SKEY_VOLUME_VALUE, SGROUP_PLAYER, 50).toInt();

    m_volume_slider->setValue(volumeValue);

    auto playerPos = loadParameter(SKEY_PLAYER_POS, SGROUP_PLAYER, 0)
            .toLongLong();
    m_player->setPosition(playerPos);

    auto isPlayerPaused = loadParameter(SKEY_IS_PLAYER_PAUSED, SGROUP_PLAYER,
                                        true)
                .toBool();
    if(!isPlayerPaused)
        m_player->play();


}

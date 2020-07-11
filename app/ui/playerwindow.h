#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QWidget>
//#include <QMainWindow>
#include <QMediaPlayer>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QIcon>


class QLineEdit;
class PlaylistWindow;
class VideoWidget;
class QProgressBar;
class QSlider;
class QLabel;

class PlayerWindow : public QWidget
{
    Q_OBJECT

public:
    PlayerWindow(QWidget *parent = nullptr);
    ~PlayerWindow() override;

private slots:
    void on_actionOpen_triggered();
    void on_actionPlay_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();
    void update_volume_label(int curr_vol);
    void change_volume_level(int delta);
    void update_seek_slider_value(qint64 curr_player_pos);
    void update_player_pos(int seek_slider_pos);
    void update_time_labels(int curr_player_pos);
    void update_title(QString newTitle);
    //void statusChanged(QMediaPlayer::MediaStatus status);
    //void handleCursor(QMediaPlayer::MediaStatus status);
    //void setStatusInfo(const QString &info);
    void on_action_seek_forwards_triggered();
    void on_action_seek_backwards_triggered();
    void on_action_show_playlist_triggered();
    void on_action_prev_triggered();
    void on_action_next_triggered();
    void save_settings();
    void load_settings();

private:
    QMediaPlayer *m_player;
    PlaylistWindow *m_pls_window;
    VideoWidget *m_video_widget;
    QLabel *m_current_play_time_label;
    QProgressBar *m_seek_bar;
    QLabel *m_remaining_time_label;
    QSlider *m_seek_slider;
    QLabel *m_volume_label;
    QSlider *m_volume_slider;
    void displayErrorMessage(const QString &msg);
    QToolBar *mainToolBar;
    QAction *actionOpen;
    QAction *actionPlay;
    QAction *actionPause;
    QAction *actionStop;
    QAction *action_seek_backwards;
    QAction *action_seek_forwards;
    QAction *action_show_playlist;
    QAction *action_prev;
    QAction *action_next;
    QStatusBar *statusBar;
    QIcon actionOpenIcon;
    QIcon actionPlayIcon;
    QIcon actionPauseIcon;
    QIcon actionStopIcon;
    QIcon actionSeekBackwardIcon;
    QIcon actionSeekForwardIcon;
    QIcon actionPrevIcon;
    QIcon actionNextIcon;
    QIcon actionPlaylistIcon;


protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    //void resizeEvent(QResizeEvent *event) override;
};

#endif // PLAYERWINDOW_H

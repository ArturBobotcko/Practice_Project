#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QMediaMetaData>
#include <QThread>
#include <QRandomGenerator>
#include <QtSql>
#include <QMessageBox>
#include <QMenu>
#include <QCloseEvent>
#include "musicplayer.h"

namespace Ui {
class PlaylistWindow;
}

class PlaylistWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistWindow(QWidget *parent = nullptr);
    ~PlaylistWindow();
    void setPlaylistName(const QString& playlistName);
    void setIntefaceStyle();
    void insertTracks();

private slots:
    void playBtn_clicked();
    void stopTrackBtn_clicked();
    void prevTrackBtn_clicked();
    void nextTrackBtn_clicked();
    void muteBtn_clicked();
    void repeatBtn_clicked();
    void mixBtn_clicked();
    void trackSlider_sliderMoved(int position);
    void volumeSlider_sliderMoved(int position);
    void trackSlider_valueChanged(int value);
    void setDuration();
    void cellDoubleClicked(int iRow, int iColumn);
    void playTrack();
    void changedPlaybackState();
    void up_buttonClicked();
    void down_buttonClicked();
    void showContextMenu();
    void deleteTrack(int actionId, const QModelIndexList &selectedRows);
    void closeEvent(QCloseEvent *event);

private:
    Ui::PlaylistWindow *ui;
    musicPlayer& m_player = musicPlayer::instance();
    QWidget* mainWindow;
};

#endif // PLAYLISTWINDOW_H

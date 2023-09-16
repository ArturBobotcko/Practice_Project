#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileDialog>
#include <QDebug>
#include <QMediaMetaData>
#include <QThread>
#include <QRandomGenerator>
#include "ui_mainwindow.h"
#include "add_playlist.h"
#include "playlistwindow.h"
#include "selectplaylist.h"
#include "musicplayer.h"

class Ui_MainWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setIntefaceStyle();
    void insertPlaylists();
    void insertTracks();
    QStringList sendData();

private slots:
    void addTracks_clicked();
    void addPlaylistBtn_clicked();
    void deleteBtn_clicked();
    void trackSlider_sliderMoved(int position);
    void volumeSlider_sliderMoved(int position);
    void trackSlider_valueChanged(int value);
    void cellDoubleClicked(int iRow, int iColumn);
    void playlist_list_cellClicked(int row, int column);
    void playlistDoubleClicked();
    void retrieveMetadata();
    void up_buttonClicked();
    void down_buttonClicked();
    void showContextMenu();
    void setDuration();
    void changedPlaybackState();
    void autoPlay();
    void playBtn_clicked();
    void stopTrackBtn_clicked();
    void playTrack();
    void prevTrackBtn_clicked();
    void nextTrackBtn_clicked();
    void muteBtn_clicked();
    void repeatBtn_clicked();
    void mixBtn_clicked();

private:
    Ui_MainWindow *ui;
    int pause_position;
    add_playlist* playlist_window = nullptr;
    QString selectedPlaylist;
    int rowOnDelete;
    int current_volume;
    PlaylistWindow* playlist = nullptr;
    void addTrackToPlaylist(int actionId, const QModelIndexList& selectedRows);
    selectPlaylist* selectPlaylistDialog = nullptr;
    QStringList rowValues;
    void deleteTrack(int actionId, const QModelIndexList& selectedRows);
    void setupTableWidgetTooltips();
    musicPlayer& m_player = musicPlayer::instance();
};
#endif // MAINWINDOW_H

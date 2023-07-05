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
    void playBtn_clicked();
    void stopTrackBtn_clicked();
    void prevTrackBtn_clicked();
    void nextTrackBtn_clicked();
    void muteBtn_clicked();
    void addTracks_clicked();
    void addPlaylistBtn_clicked();
    void deleteBtn_clicked();
    void repeatBtn_clicked();
    void mixBtn_clicked();
    void trackSlider_sliderMoved(int position);
    void volumeSlider_sliderMoved(int position);
    void trackSlider_valueChanged(int value);
    void setDuration();
    void cellDoubleClicked(int iRow, int iColumn);
    void playTrack();
    void playlist_list_cellClicked(int row, int column);
    void volumeSlider_valueChanged(int value);
    void autoPlay();
    void changedPlaybackState();
    void playlistDoubleClicked();
    void retrieveMetadata();
    void up_buttonClicked();
    void down_buttonClicked();
    void showContextMenu();
private:
    Ui_MainWindow *ui;
    QMediaPlayer *m_player;
    QAudioOutput* m_audioOutput;
    int pause_position;
    add_playlist* playlist_window = nullptr;
    QString selectedPlaylist;
    int rowOnDelete;
    int current_volume;
    bool muted = false;
    //Playlist* playlist = nullptr;
    PlaylistWindow* playlist = nullptr;
    void addTrackToPlaylist(int actionId, const QModelIndexList& selectedRows);
    selectPlaylist* selectPlaylistDialog = nullptr;
    QStringList rowValues;
    bool mix;
    void deleteTrack(int actionId, const QModelIndexList& selectedRows);
};
#endif // MAINWINDOW_H

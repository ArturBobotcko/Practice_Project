#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileDialog>
#include <QDebug>
#include <QMediaMetaData>
#include "ui_mainwindow.h"
#include "add_playlist.h"

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
    void MetaDataAvailable();
    void trackSlider_valueChanged(int value);
    void setDuration();
    void cellDoubleClicked(int iRow, int iColumn);
    void playTrack();
    void playlist_list_cellClicked(int row, int column);
    void volumeSlider_valueChanged(int value);
    void autoPlay();
    void changedPlaybackState();
private:
    Ui_MainWindow *ui;
    QMediaPlayer *m_player;
    QAudioOutput* m_audioOutput;
    int current_track;
    int pause_position;
    add_playlist* playlist_window = nullptr;
    QString selectedPlaylist;
    int rowOnDelete;
    int current_volume;
    bool muted = false;
};
#endif // MAINWINDOW_H

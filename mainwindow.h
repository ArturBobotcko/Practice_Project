#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileDialog>
#include <QDebug>
#include <QMediaMetaData>
#include "ui_mainwindow.h"
#include "add_playlist.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setIntefaceStyle();
    void insertPlaylists();

private slots:
    void on_playBtn_clicked();
    void on_addTracks_clicked();
    void on_stopTrackBtn_clicked();
    void on_prevTrackBtn_clicked();
    void on_muteBtn_clicked();
    void on_trackSlider_sliderMoved(int position);
    void on_volumeSlider_sliderMoved(int position);
    void onMetaDataAvailable();
    void on_nextTrackBtn_clicked();
    void on_trackSlider_valueChanged(int value);
    void setDuration();
    void on_addPlaylistBtn_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *m_player;
    QAudioOutput* m_audioOutput;
    int current_track;
    int pause_position;
    int current_position;
    bool onPause;
    add_playlist* playlist_window = nullptr;

};
#endif // MAINWINDOW_H

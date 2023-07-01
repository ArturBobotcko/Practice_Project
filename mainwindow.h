#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileDialog>
#include <QDebug>
#include <QMediaMetaData>
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
    void on_deleteBtn_clicked();
    void on_playlist_list_cellClicked(int row, int column);

private:
    Ui_MainWindow *ui;
    QMediaPlayer *m_player;
    QAudioOutput* m_audioOutput;
    int current_track;
    int pause_position;
    int current_position;
    bool onPause;
    add_playlist* playlist_window = nullptr;
    QString selectedPlaylist;
    int rowOnDelete;
    int current_volume;
    bool muted = false;
};
#endif // MAINWINDOW_H

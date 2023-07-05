#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileDialog>
#include <QDebug>
#include <QMediaMetaData>
#include <QThread>
#include <QRandomGenerator>
#include <QtSql>
#include <QMessageBox>
#include <QMenu>

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
    //void addTracks_clicked();
    //void addPlaylistBtn_clicked();
    //void deleteBtn_clicked();
    void repeatBtn_clicked();
    void mixBtn_clicked();
    void trackSlider_sliderMoved(int position);
    void volumeSlider_sliderMoved(int position);
    void trackSlider_valueChanged(int value);
    void setDuration();
    void cellDoubleClicked(int iRow, int iColumn);
    void playTrack();
    //void playlist_list_cellClicked(int row, int column);
    void volumeSlider_valueChanged(int value);
    void autoPlay();
    void changedPlaybackState();
    //void playlistDoubleClicked();
    //void retrieveMetadata();
    //void up_buttonClicked();
    //void down_buttonClicked();
    void showContextMenu();
    void deleteTrack(int actionId, const QModelIndexList &selectedRows);

private:
    Ui::PlaylistWindow *ui;
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
    bool mix;
    int pause_position;
    bool muted = false;
    int current_volume;
};

#endif // PLAYLISTWINDOW_H

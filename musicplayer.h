#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QAudioOutput>

class musicPlayer
{
public:
    musicPlayer();
    ~musicPlayer();
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
    void setDuration();
    void playTrack();
    void autoPlay();
    void changedPlaybackState();
    void volumeSlider_sliderMoved(int position);
    void volumeSlider_valueChanged(int value);
private:
    QMediaPlayer *m_player;
    QAudioOutput* m_audioOutput;
    int pause_position;
    int current_volume;
    bool muted = false;
    bool mix;
};

#endif // MUSICPLAYER_H

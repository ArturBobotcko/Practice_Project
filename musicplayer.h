#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QString>
#include <QMediaMetaData>

class musicPlayer
{
public:
    static musicPlayer& instance();
    ~musicPlayer();
    QMediaMetaData get_metadata();
    QMediaPlayer::PlaybackState get_playbackState();
    qint64 get_position();
    qint64 get_duration();
    void set_position(qint64 pos);
    void set_source(const QUrl& path);
    void play_track();
    void stop_track();
    bool is_mixed();
    bool is_muted();
    bool is_repeated();
    void set_mix();
    void set_mute();
    void set_repeat();
    void set_volume(float volume);
    QMediaPlayer* get_player();

private:
    musicPlayer();
    QMediaPlayer *m_player;
    QAudioOutput* m_audioOutput;
    QString m_path;
    int pause_position;
    int current_volume;
    bool muted;
    bool mixed;
    bool repeated;
};

#endif // MUSICPLAYER_H

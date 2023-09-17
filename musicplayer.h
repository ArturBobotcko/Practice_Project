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
    quint64 get_pause_position();
    quint64 get_current_volume();
    void set_position(qint64 pos);
    void set_source(const QUrl& path);
    void set_mix();
    void set_mute();
    void set_repeat();
    void set_volume(float volume);
    void set_pause_position(quint64 position);
    void set_current_volume(quint64 position);
    void play_track();
    void stop_track();
    bool is_mixed();
    bool is_muted();
    bool is_repeated();
    QMediaPlayer* get_player();

private:
    musicPlayer();
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
    QString m_path;
    qint64 pause_position;
    qint64 current_volume;
    bool muted;
    bool mixed;
    bool repeated;
};

#endif // MUSICPLAYER_H

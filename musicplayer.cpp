#include "musicplayer.h"

musicPlayer& musicPlayer::instance()
{
    static musicPlayer instance;
    return instance;
}

musicPlayer::musicPlayer()
{
    m_player = new QMediaPlayer;
    m_audioOutput = new QAudioOutput;
    m_player->setAudioOutput(m_audioOutput);
    mixed = false;
    muted = false;
    pause_position = 0;
}

musicPlayer::~musicPlayer()
{
    delete m_player;
    delete m_audioOutput;
}

QMediaMetaData musicPlayer::get_metadata()
{
    return m_player->metaData();
}

QMediaPlayer::PlaybackState musicPlayer::get_playbackState()
{
    return m_player->playbackState();
}

qint64 musicPlayer::get_position()
{
    return m_player->position();
}

qint64 musicPlayer::get_duration()
{
    return m_player->duration();
}

void musicPlayer::set_position(qint64 pos)
{
    m_player->setPosition(pos);
}

void musicPlayer::play_track()
{
    m_player->play();
}

void musicPlayer::stop_track()
{
    m_player->stop();
}

bool musicPlayer::is_mixed()
{
    return mixed;
}

bool musicPlayer::is_repeated()
{
    return repeated;
}

bool musicPlayer::is_muted()
{
    return muted;
}

void musicPlayer::set_mix()
{
    mixed = !mixed;
}

void musicPlayer::set_mute()
{
    muted = !muted;
}

void musicPlayer::set_repeat()
{
    repeated = !repeated;
}

void musicPlayer::set_volume(float volume)
{
    m_audioOutput->setVolume(volume);
}

QMediaPlayer *musicPlayer::get_player()
{
    return m_player;
}

void musicPlayer::set_source(const QUrl& path)
{
    m_player->setSource(path);
}

#include "musicplayer.h"

musicPlayer::musicPlayer()
{
    m_player = new QMediaPlayer;
    m_audioOutput = new QAudioOutput;
    m_player->setAudioOutput(m_audioOutput);
    mix = false;
    pause_position = 0;
}

musicPlayer::~musicPlayer()
{
    delete m_player;
    delete m_audioOutput;
}

void musicPlayer::playTrack(QString& path)
{
    m_player->stop();
    m_player->setSource(path);
    //m_player->setSource(QUrl::fromLocalFile(item->text())); // path variable ?
    m_player->play();
}

void musicPlayer::playBtn_clicked()
{
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        pause_position = m_player->position();
        m_player->stop();
    } else {
        //m_player->setPosition(ui->trackSlider->value()); // wtf
        playTrack();
    }
}

void musicPlayer::stopTrackBtn_clicked()
{
    m_player->stop();
    pause_position = 0;
}

void musicPlayer::prevTrackBtn_clicked()
{
    playBtn_clicked();
}

void musicPlayer::nextTrackBtn_clicked()
{
    playBtn_clicked();
}

void musicPlayer::repeatBtn_clicked()
{
    if (m_player->loops() == QMediaPlayer::Once) {
        m_player->setLoops(QMediaPlayer::Infinite);
    } else {
        m_player->setLoops(QMediaPlayer::Once);
    }
}

void musicPlayer::mixBtn_clicked()
{
    if (mix) {
        mix = false;
    } else {
        mix = true;
    }
}

void musicPlayer::autoPlay()
{
    if (m_player->mediaStatus() == m_player->EndOfMedia) {
        nextTrackBtn_clicked();
    }
}

void musicPlayer::muteBtn_clicked()
{
    if (!muted)
    {
        m_audioOutput->setVolume(0);
        muted = true;
    }
    else
    {
        m_audioOutput->setVolume(current_volume/100.0f);
        muted = false;
    }
}

void musicPlayer::volumeSlider_sliderMoved(int position)
{
    m_audioOutput->setVolume(position/100.0f);
}

void musicPlayer::volumeSlider_valueChanged(int value)
{
    m_audioOutput->setVolume(value/100.0f);
}

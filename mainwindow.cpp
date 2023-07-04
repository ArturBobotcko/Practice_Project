#include "mainwindow.h"
#include "StyleHelper.h"
#include "databasehandler.h"
//#include "threadplayer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    DataBaseHandler& dbhandler = DataBaseHandler::instance();
    dbhandler.createDataBase();
    ui->setupUi(this);
    setIntefaceStyle();
    insertPlaylists();
    insertTracks();

    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);

    current_track = 0;
    pause_position = 0;

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->volumeSlider->setValue(50);

    connect(m_player, &QMediaPlayer::metaDataChanged, this, &MainWindow::MetaDataAvailable);
    connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::trackSlider_valueChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::setDuration);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::autoPlay);
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, &MainWindow::changedPlaybackState);

    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::cellDoubleClicked);
    connect(ui->playlist_list, &QTableWidget::cellClicked, this, &MainWindow::playlist_list_cellClicked);

    connect(ui->playBtn, &QPushButton::clicked, this, &MainWindow::playBtn_clicked);
    connect(ui->addPlaylistBtn, &QPushButton::clicked, this, &MainWindow::addPlaylistBtn_clicked);

    connect(ui->stopTrackBtn, &QPushButton::clicked, this, &MainWindow::stopTrackBtn_clicked);
    connect(ui->prevTrackBtn, &QPushButton::clicked, this, &MainWindow::prevTrackBtn_clicked);
    connect(ui->muteBtn, &QPushButton::clicked, this, &MainWindow::muteBtn_clicked);
    connect(ui->nextTrackBtn, &QPushButton::clicked, this, &MainWindow::nextTrackBtn_clicked);
    connect(ui->addPlaylistBtn, &QPushButton::clicked, this, &MainWindow::addPlaylistBtn_clicked);
    connect(ui->deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteBtn_clicked);
    connect(ui->actionOpen_file, &QAction::triggered, this, &MainWindow::addTracks_clicked);
    connect(ui->repeatButton, &QPushButton::clicked, this, &MainWindow::repeatBtn_clicked);
    connect(ui->mixButton, &QPushButton::clicked, this, &MainWindow::mixBtn_clicked);

    connect(ui->trackSlider, &QSlider::sliderMoved, this, &MainWindow::trackSlider_sliderMoved);
    connect(ui->volumeSlider, &QSlider::sliderMoved, this, &MainWindow::volumeSlider_sliderMoved);
    connect(ui->trackSlider, &QSlider::valueChanged, this, &MainWindow::trackSlider_valueChanged);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::volumeSlider_valueChanged);
    connect(ui->playlist_list, &QTableWidget::cellDoubleClicked, this, &MainWindow::playlistDoubleClicked);
    // idk where to put it
    ui->mixButton->setIconSize(QSize(25, 25));
    ui->repeatButton->setIconSize(QSize(25, 25));
}

void MainWindow::playTrack()
{
    m_player->stop();
    ui->tableWidget->selectRow(current_track);
    QTableWidgetItem *item = ui->tableWidget->item(current_track, 0);
    if (!item) {
        return;
    }
    m_player->setSource(QUrl::fromLocalFile(item->text()));
    m_player->play();
}

void MainWindow::setIntefaceStyle()
{
    StyleHelper::setAllButtonsStyle(ui);
    StyleHelper::setAllSlidersStyle(ui);
    StyleHelper::setBackgroundStyle(ui);
    StyleHelper::setLablesStyle(ui);
}

void MainWindow::cellDoubleClicked(int iRow, int iColumn)
{
    current_track = iRow;
    playTrack();
}
// Функция вставки списка плейлистов в виджет таблицы
void MainWindow::insertPlaylists()
{
    QSqlQueryModel* playlists = DataBaseHandler::instance().getPlaylists();
    ui->playlist_list->setRowCount(playlists->rowCount());
    ui->playlist_list->setColumnCount(1);
    ui->playlist_list->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QStringList headerLabels;
    headerLabels << "Название плейлиста";
    ui->playlist_list->setHorizontalHeaderLabels(headerLabels);
    for (int row = 0; row < playlists->rowCount(); ++row)
    {
        QString playlist_name = playlists->record(row).value("playlist_name").toString();
        qDebug() << playlist_name;
        QTableWidgetItem* item = new QTableWidgetItem(playlist_name);
        ui->playlist_list->setItem(row, 0, item);

    }
    delete playlists;
}


void MainWindow::insertTracks()
{
    QSqlQueryModel* track= DataBaseHandler::instance().getTracks();
    ui->tableWidget->setRowCount(track->rowCount());
    //ui->tableWidget->setColumnCount(1);
   // ui->tableWidget->horizontalHeader()->tableWidget(QHeaderView::Stretch);
    //QStringList headerLabels;
    //headerLabels << "Название ";
    //ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    for (int row = 0; row < track->rowCount(); ++row)
    {
        QString path = track->record(row).value("path").toString();
        QString track_name = track->record(row).value("track_name").toString();
        QString author = track->record(row).value("author").toString();
        QString duration = track->record(row).value("duration").toString();
        //qDebug() << path;
        //qDebug() << track_name;
        QTableWidgetItem* item = new QTableWidgetItem(path);
        QTableWidgetItem* item2 = new QTableWidgetItem(track_name);
        QTableWidgetItem* item3 = new QTableWidgetItem(author);
        QTableWidgetItem* item4 = new QTableWidgetItem(duration);
        ui->tableWidget->setItem(row, 0, item);
        ui->tableWidget->setItem(row, 1, item2);
        ui->tableWidget->setItem(row, 2, item3);
        ui->tableWidget->setItem(row, 3, item4);
    }
    //delete tableWidget;
}
void MainWindow::MetaDataAvailable()
{
    QMediaMetaData data = m_player->metaData();
    ui->tableWidget->setItem(current_track, 1, new QTableWidgetItem(data.stringValue(QMediaMetaData::Title)));
    ui->tableWidget->setItem(current_track, 2, new QTableWidgetItem(data.stringValue(QMediaMetaData::Author)));
    ui->tableWidget->setItem(current_track, 3, new QTableWidgetItem(data.stringValue(QMediaMetaData::Duration)));
    DataBaseHandler::instance().addTrack(data.stringValue(QMediaMetaData::Url), data.stringValue(QMediaMetaData::Title), data.stringValue(QMediaMetaData::Author), data.stringValue(QMediaMetaData::Duration));
}

QStringList MainWindow::sendData()
{
    QMediaMetaData data = m_player->metaData();
    QStringList meta = {data.stringValue(QMediaMetaData::Title), data.stringValue(QMediaMetaData::Author), data.stringValue(QMediaMetaData::Duration)};
    return meta;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_player;
    delete m_audioOutput;
    delete playlist_window;
    delete playlist;
}

void MainWindow::playBtn_clicked()
{
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        pause_position = m_player->position();
        m_player->stop();
    } else {
        m_player->setPosition(ui->trackSlider->value());
        playTrack();
    }
}

void MainWindow::changedPlaybackState()
{
    QIcon icon;
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        icon.addFile(QString::fromUtf8(":/new/prefix1/resources/pause.svg"), QSize(), QIcon::Normal, QIcon::Off);
    } else {
        icon.addFile(QString::fromUtf8(":/new/prefix1/resources/play.svg"), QSize(), QIcon::Normal, QIcon::Off);
        ui->trackSlider->setValue(pause_position);
    }
    ui->playBtn->setIcon(icon);
    ui->playBtn->setIconSize(QSize(50, 50));
    ui->playBtn->setFlat(false);
}

void MainWindow::playlistDoubleClicked()
{
    qDebug() << selectedPlaylist;
    if(!playlist)
    {
        playlist = new PlaylistWindow(this);
    }
    playlist->setPlaylistName(selectedPlaylist);
    playlist->show();
}

void MainWindow::addTracks_clicked()
{
    QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::ExistingFiles);

        QStringList selected_files = dialog.getOpenFileNames();
        foreach(const QString& file, selected_files) {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(file));


            QThread* thread = new QThread(this);


            QMediaPlayer* player = new QMediaPlayer;
            player->setSource(QUrl::fromLocalFile(file));
            player->moveToThread(thread);


            QObject::connect(thread, &QThread::started, player, &QMediaPlayer::play);


            QObject::connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::retrieveMetadata);


            QObject::connect(thread, &QThread::finished, player, &QMediaPlayer::stop);


            QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);


            thread->start();
        }

}

void MainWindow::retrieveMetadata()
{
    //if (status == QMediaPlayer::LoadedMedia) {
            QMediaPlayer* player = qobject_cast<QMediaPlayer*>(sender());
            if (player) {
                QMediaMetaData data = player->metaData();
                qDebug() << "Title:" << data.value(QMediaMetaData::Title).toString();
                qDebug() << "Author:" << data.value(QMediaMetaData::Author).toString();
                qDebug() << "Duration:" << data.stringValue(QMediaMetaData::Duration);
            } else {
                qDebug() << "empty :(";
            }
    //} else {
       // qDebug () << "wtf";
   // }
}

void MainWindow::stopTrackBtn_clicked()
{
    m_player->stop();
}

void MainWindow::prevTrackBtn_clicked()
{
    if (current_track == 0) {
        current_track = ui->tableWidget->rowCount();
    }
    --current_track;
    playTrack();
}

void MainWindow::nextTrackBtn_clicked()
{
    if (current_track == ui->tableWidget->rowCount() - 1) {
        current_track = -1; // i`m stupud but i`m free
    }
    ++current_track;
    playTrack();
}

void MainWindow::repeatBtn_clicked()
{
    if (m_player->loops() == QMediaPlayer::Once) {
        ui->repeatButton->setIcon(QIcon(":/new/prefix1/resources/repeat.svg"));
        m_player->setLoops(QMediaPlayer::Infinite);
    } else {
        ui->repeatButton->setIcon(QIcon(":/new/prefix1/resources/no-repeat.svg"));
        m_player->setLoops(QMediaPlayer::Once);
    }
}

void MainWindow::mixBtn_clicked()
{
    //this->close();
}

void MainWindow::autoPlay()
{
    if (m_player->mediaStatus() == m_player->EndOfMedia) {
        nextTrackBtn_clicked();
    }
}

void MainWindow::setDuration()
{
    ui->trackSlider->setMaximum(m_player->duration());
}

void MainWindow::muteBtn_clicked()
{
    QIcon high_vol_icon, mid_vol_icon, low_vol_icon, muted_vol_icon;
    high_vol_icon.addFile(QString::fromUtf8(":/new/prefix1/resources/high-volume.svg"), QSize(), QIcon::Normal, QIcon::Off);
    mid_vol_icon.addFile(QString::fromUtf8(":/new/prefix1/resources/mid-volume.svg"), QSize(), QIcon::Normal, QIcon::Off);
    low_vol_icon.addFile(QString::fromUtf8(":/new/prefix1/resources/low-volume.svg"), QSize(), QIcon::Normal, QIcon::Off);
    muted_vol_icon.addFile(QString::fromUtf8(":/new/prefix1/resources/muted-volume.svg"), QSize(), QIcon::Normal, QIcon::Off);
    if (!muted)
    {
        current_volume = ui->volumeSlider->value();
        ui->muteBtn->setIcon(muted_vol_icon);
        m_audioOutput->setVolume(0);
        ui->volumeSlider->setValue(0);
        muted = true;
    }
    else
    {
        if (current_volume < 100 && current_volume >= 75)
        {
            ui->muteBtn->setIcon(high_vol_icon);
        }
        else if (current_volume < 75 && current_volume >= 25)
        {
            ui->muteBtn->setIcon(mid_vol_icon);
        }
        else if (current_volume < 25 && current_volume != 0)
        {
            ui->muteBtn->setIcon(low_vol_icon);
        }
        m_audioOutput->setVolume(current_volume/100.0f);
        ui->volumeSlider->setValue(current_volume);
        muted = false;
    }
}

void MainWindow::trackSlider_sliderMoved(int position)
{
    m_player->setPosition(position);
}

void MainWindow::volumeSlider_sliderMoved(int position)
{
    float volume = position/100.0f;
    QIcon high_vol_icon, mid_vol_icon, low_vol_icon, muted_vol_icon;
    high_vol_icon.addFile(QString::fromUtf8(":/new/prefix1/resources/high-volume.svg"), QSize(), QIcon::Normal, QIcon::Off);
    mid_vol_icon.addFile(QString::fromUtf8(":/new/prefix1/resources/mid-volume.svg"), QSize(), QIcon::Normal, QIcon::Off);
    low_vol_icon.addFile(QString::fromUtf8(":/new/prefix1/resources/low-volume.svg"), QSize(), QIcon::Normal, QIcon::Off);
    muted_vol_icon.addFile(QString::fromUtf8(":/new/prefix1/resources/muted-volume.svg"), QSize(), QIcon::Normal, QIcon::Off);
    ui->muteBtn->setIconSize(QSize(25, 25));
    ui->muteBtn->setFlat(false);
    if (position < 100 && position >= 75)
    {
        ui->muteBtn->setIcon(high_vol_icon);
    }
    else if (position < 75 && position >= 25)
    {
        ui->muteBtn->setIcon(mid_vol_icon);
    }
    else if (position < 25 && position != 0)
    {
        ui->muteBtn->setIcon(low_vol_icon);
    }
    else if (position == 0)
    {
        ui->muteBtn->setIcon(muted_vol_icon);
    }
    m_audioOutput->setVolume(volume);
}

void MainWindow::trackSlider_valueChanged(int value)
{
    ui->trackSlider->setValue(value);
}

// Функция вызова окна для добавления плейлиста
void MainWindow::addPlaylistBtn_clicked()
{
    if(!playlist_window)
    {
        playlist_window = new add_playlist(this);
    }
    playlist_window->show();
}

// Функция удаления плейлиста по кнопке
void MainWindow::deleteBtn_clicked()
{
    qDebug() << selectedPlaylist;
    DataBaseHandler::instance().deletePlaylist(selectedPlaylist);
    if (!selectedPlaylist.isEmpty())
    {
        ui->playlist_list->removeRow(rowOnDelete);
    }
    selectedPlaylist.clear();
}

// Функция выбора имени плейлиста по клику на него
void MainWindow::playlist_list_cellClicked(int row, int column)
{
    QString value = ui->playlist_list->item(row, column)->text();
    selectedPlaylist = value;
    rowOnDelete = row;
}

void MainWindow::volumeSlider_valueChanged(int value)
{
    float volume = value/100.0f;
    QIcon high_vol_icon, mid_vol_icon, low_vol_icon, muted_vol_icon;
    high_vol_icon.addFile(QString::fromUtf8(":/new/prefix1/resources/high-volume.svg"), QSize(), QIcon::Normal, QIcon::Off);
    mid_vol_icon.addFile(QString::fromUtf8(":/new/prefix1/resources/mid-volume.svg"), QSize(), QIcon::Normal, QIcon::Off);
    low_vol_icon.addFile(QString::fromUtf8(":/new/prefix1/resources/low-volume.svg"), QSize(), QIcon::Normal, QIcon::Off);
    muted_vol_icon.addFile(QString::fromUtf8(":/new/prefix1/resources/muted-volume.svg"), QSize(), QIcon::Normal, QIcon::Off);
    ui->muteBtn->setIconSize(QSize(25, 25));
    ui->muteBtn->setFlat(false);
    if (value < 100 && value >= 75)
    {
        ui->muteBtn->setIcon(high_vol_icon);
    }
    else if (value < 75 && value >= 25)
    {
        ui->muteBtn->setIcon(mid_vol_icon);
    }
    else if (value < 25 && value != 0)
    {
        ui->muteBtn->setIcon(low_vol_icon);
    }
    else if (value == 0)
    {
        ui->muteBtn->setIcon(muted_vol_icon);
    }
    m_audioOutput->setVolume(volume);
}

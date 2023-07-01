#include "mainwindow.h"
#include "StyleHelper.h"
#include "databasehandler.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    DataBaseHandler& dbhandler = DataBaseHandler::instance();
    dbhandler.createDataBase();
    ui->setupUi(this);
    setIntefaceStyle();
    insertPlaylists();

    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);

    current_track = 0;
    pause_position = 0;
    current_position = 0;
    onPause = true;
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->volumeSlider->setValue(50);
    connect(m_player, &QMediaPlayer::metaDataChanged, this, &MainWindow::onMetaDataAvailable);
    connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::on_trackSlider_valueChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::setDuration);
    connect(ui->addPlaylistBtn, &QPushButton::clicked, this, &MainWindow::on_addPlaylistBtn_clicked);
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::cellDoubleClicked);
}

void MainWindow::playTrack()
{
    m_player->stop();
    ui->tableWidget->selectRow(current_track);
    QTableWidgetItem *item = ui->tableWidget->item(current_track, 0);
    if (item) {
        m_player->setSource(QUrl::fromLocalFile(item->text()));
    } else {
        return;
    }
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

void MainWindow::onMetaDataAvailable()
{
    QMediaMetaData data = m_player->metaData();
    ui->tableWidget->setItem(current_track, 1, new QTableWidgetItem(data.stringValue(QMediaMetaData::Title)));
    ui->tableWidget->setItem(current_track, 2, new QTableWidgetItem(data.stringValue(QMediaMetaData::Author)));
    ui->tableWidget->setItem(current_track, 3, new QTableWidgetItem(data.stringValue(QMediaMetaData::Duration)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_player;
    delete m_audioOutput;
    delete playlist_window;
}

void MainWindow::on_playBtn_clicked()
{
    if (onPause) {
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/resources/pause.svg"), QSize(), QIcon::Normal, QIcon::Off);
        ui->playBtn->setIcon(icon);
        ui->playBtn->setIconSize(QSize(50, 50));
        ui->playBtn->setFlat(false);
        m_player->setPosition(pause_position);
        playTrack();
        onPause = false;
    }
    else
    {
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/resources/play.svg"), QSize(), QIcon::Normal, QIcon::Off);
        ui->playBtn->setIcon(icon);
        ui->playBtn->setIconSize(QSize(50, 50));
        ui->playBtn->setFlat(false);
        pause_position = current_position;
        m_player->stop();
        ui->trackSlider->setValue(pause_position);
        onPause = true;
    }
}

void MainWindow::on_addTracks_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    QStringList selected_files = dialog.getOpenFileNames();
    foreach(const QString& file, selected_files) {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(file));
    }
}

void MainWindow::on_stopTrackBtn_clicked()
{
    m_player->stop();
}

void MainWindow::on_prevTrackBtn_clicked()
{
    --current_track;
    playTrack();
}

void MainWindow::on_nextTrackBtn_clicked()
{
    ++current_track;
    playTrack();
}

void MainWindow::setDuration()
{
    ui->trackSlider->setMaximum(m_player->duration());
}

void MainWindow::on_muteBtn_clicked()
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

void MainWindow::on_trackSlider_sliderMoved(int position)
{
    m_player->setPosition(position);
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
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

void MainWindow::on_trackSlider_valueChanged(int value)
{
    ui->trackSlider->setValue(value);
    current_position = value;
}

// Функция вызова окна для добавления плейлиста
void MainWindow::on_addPlaylistBtn_clicked()
{
    if(!playlist_window)
    {
        playlist_window = new add_playlist(this);
    }
    playlist_window->show();
}

// Функция удаления плейлиста по кнопке
void MainWindow::on_deleteBtn_clicked()
{
    qDebug() << selectedPlaylist;
    DataBaseHandler::instance().deletePlaylist(selectedPlaylist);
    if (selectedPlaylist != "")
    {
        ui->playlist_list->removeRow(rowOnDelete);
    }
    selectedPlaylist.clear();
}

// Функция выбора имени плейлиста по клику на него
void MainWindow::on_playlist_list_cellClicked(int row, int column)
{
    QString value = ui->playlist_list->item(row, column)->text();
    selectedPlaylist = value;
    rowOnDelete = row;
}

void MainWindow::on_volumeSlider_valueChanged(int value)
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

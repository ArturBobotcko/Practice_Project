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
    current_track = 0;
    pause_position = 0;
    current_position = 0;
    onPause = true;
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  // Включаем выделение строк
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // Разрешаем выделять только одну строку
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->volumeSlider->setValue(50);
    connect(m_player, &QMediaPlayer::metaDataChanged, this, &MainWindow::onMetaDataAvailable);
    connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::on_trackSlider_valueChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::setDuration);
}

void MainWindow::setIntefaceStyle()
{
    StyleHelper::setAllButtonsStyle(ui);
    StyleHelper::setAllSlidersStyle(ui);
    StyleHelper::setBackgroundStyle(ui);
    StyleHelper::setLablesStyle(ui);
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
    ui->track_name->setText(data.stringValue(QMediaMetaData::Title));
    ui->track_duration->setText(data.stringValue(QMediaMetaData::Duration));
    ui->track_author->setText(data.stringValue(QMediaMetaData::Author));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_player;
}

void MainWindow::on_playBtn_clicked()
{
    if (onPause) {
        m_player->setAudioOutput(m_audioOutput);
        QTableWidgetItem *item = ui->tableWidget->item(current_track, 0);
        // Если item пустой, то программа падает с ошибкой segmentation fault
        if(item)
        {
            m_player->setSource(QUrl::fromLocalFile(item->text()));
        }
        else
            return;
        //QMediaMetaData data = m_player->metaData();
        //ui->track_name->setText(m_player->metaData().stringValue(QMediaMetaData::Author));
        float volume = (ui->volumeSlider->value())/100.0f;
        m_audioOutput->setVolume(volume);
        m_player->setPosition(pause_position);
        ui->tableWidget->selectRow(current_track);
        m_player->play();
        onPause = false;
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/resources/pause.svg"), QSize(), QIcon::Normal, QIcon::Off);
        ui->playBtn->setIcon(icon);
        ui->playBtn->setIconSize(QSize(50, 50));
        ui->playBtn->setFlat(false);
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
    ui->tableWidget->setColumnCount(1);

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
    m_player->stop();
    if (current_track == 0) {
        current_track = ui->tableWidget->rowCount();
    }
    QTableWidgetItem *item = ui->tableWidget->item(--current_track, 0);
    ui->tableWidget->selectRow(current_track);
    if(item)
    {
        m_player->setSource(QUrl::fromLocalFile(item->text()));
    }
    else
        return;
    m_player->play();
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

void MainWindow::on_nextTrackBtn_clicked()
{
    m_player->stop();
    if (current_track == ui->tableWidget->rowCount() - 1) {
        current_track = -1; // i`m sorry 4 being so stupid
    }
    QTableWidgetItem *item = ui->tableWidget->item(++current_track, 0);
    if (item)
    {
        m_player->setSource(QUrl::fromLocalFile(item->text()));
    }
    else
        return;
    ui->tableWidget->selectRow(current_track);
    m_player->play();
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
    selectedPlaylist = "";
}

// Функция выбора имени плейлиста по клику на него
void MainWindow::on_playlist_list_cellClicked(int row, int column)
{
    QString value = ui->playlist_list->item(row, column)->text();
    selectedPlaylist = value;
    rowOnDelete = row;
    // Добавить вывод треков в правую таблицу
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


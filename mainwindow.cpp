#include "mainwindow.h"
#include "StyleHelper.h"
#include "databasehandler.h"
#include "ui_add_playlist.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    DataBaseHandler& dbhandler = DataBaseHandler::instance();
    dbhandler.createDataBase();
    ui->setupUi(this);
    setIntefaceStyle();

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
    //connect(ui->playBtn, &QPushButton::clicked, this, &MainWindow::on_playBtn_clicked);
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
}

void MainWindow::on_playBtn_clicked()
{
    if (onPause) {
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/resources/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->playBtn->setIcon(icon);
        ui->playBtn->setIconSize(QSize(50, 50));
        ui->playBtn->setFlat(false);
        playTrack();
        onPause = false;
    }
    else
    {
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/resources/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->playBtn->setIcon(icon);
        ui->playBtn->setIconSize(QSize(50, 50));
        ui->playBtn->setFlat(false);
        m_player->stop();
        pause_position = current_position;
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
    m_audioOutput->setVolume(0);
    ui->volumeSlider->setValue(0);
}

void MainWindow::on_trackSlider_sliderMoved(int position)
{
    m_player->setPosition(position);
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    m_audioOutput->setVolume(position/100.0f);
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

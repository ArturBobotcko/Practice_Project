#include "mainwindow.h"
#include "StyleHelper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setIntefaceStyle();
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
    //connect(ui->playBtn, &QPushButton::clicked, this, &MainWindow::on_playBtn_clicked);
}

void MainWindow::setIntefaceStyle()
{
    StyleHelper::setAllButtonsStyle(ui);
    StyleHelper::setAllSlidersStyle(ui);
    StyleHelper::setBackgroundStyle(ui);
    StyleHelper::setLablesStyle(ui);
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
        m_player->setSource(QUrl::fromLocalFile(item->text()));
        float volume = (ui->volumeSlider->value())/100.0f;
        m_audioOutput->setVolume(volume);
        m_player->setPosition(pause_position);
        ui->tableWidget->selectRow(current_track);
        m_player->play();
        onPause = false;
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/resources/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui->playBtn->setIcon(icon);
        ui->playBtn->setIconSize(QSize(50, 50));
        ui->playBtn->setFlat(false);
    } else {
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/resources/play.png"), QSize(), QIcon::Normal, QIcon::Off);
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
    m_player->setSource(QUrl::fromLocalFile(item->text()));
    ui->tableWidget->selectRow(current_track);
    m_player->play();
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

void MainWindow::on_nextTrackBtn_clicked()
{
    m_player->stop();
    if (current_track == ui->tableWidget->rowCount() - 1) {
        current_track = -1; // i`m sorry 4 being so stupid
    }
    QTableWidgetItem *item = ui->tableWidget->item(++current_track, 0);
    m_player->setSource(QUrl::fromLocalFile(item->text()));
    ui->tableWidget->selectRow(current_track);
    m_player->play();
}

void MainWindow::on_trackSlider_valueChanged(int value)
{
    ui->trackSlider->setValue(value);
    current_position = value;
}

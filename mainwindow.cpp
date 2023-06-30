#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "StyleHelper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setIntefaceStyle();
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    current_track = 0; //!!!
    //connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::on_positionChanged);
    //connect(m_audioOutput, &QMediaPlayer::durationChanged, this, &MainWindow::on_durationChanged);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  // Включаем выделение строк
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // Разрешаем выделять только одну строку
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_player, &QMediaPlayer::metaDataChanged, this, &MainWindow::onMetaDataAvailable);
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
    qDebug() << data.stringValue(QMediaMetaData::Title);
    qDebug() << m_player->metaData().stringValue(QMediaMetaData::Author);
    qDebug() << m_player->metaData().stringValue(QMediaMetaData::Duration);
    //disconnect(m_player, &QMediaPlayer::metaDataChanged, this, &MainWindow::onMetaDataAvailable);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_player;
}

void MainWindow::on_playBtn_clicked()
{
    m_player->setAudioOutput(m_audioOutput);
    QTableWidgetItem *item = ui->tableWidget->item(current_track, 0);
    m_player->setSource(QUrl::fromLocalFile(item->text()));
    //QMediaMetaData data = m_player->metaData();
    //ui->track_name->setText(m_player->metaData().stringValue(QMediaMetaData::Author));
    m_player->play();
}

/*void MainWindow::on_actionOpen_File_triggered()
{
    //QString FileName = QFileDialog::getOpenFileName(this, tr("Select Audio"), "", tr("MP3 Files (*.MP3)"));

}*/

void MainWindow::on_addTracks_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    ui->tableWidget->setColumnCount(1);
    //QString fileName = QFileDialog::getOpenFileName(this,
      // tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));

    QStringList selected_files = dialog.getOpenFileNames();
    foreach(const QString& file, selected_files) {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(file));
    }
    //decode
}


void MainWindow::on_stopTrackBtn_clicked()
{
    m_player->stop();
}


void MainWindow::on_prevTrackBtn_clicked()
{
    m_player->stop();
    QTableWidgetItem *item = ui->tableWidget->item(--current_track, 0);
    m_player->setSource(QUrl::fromLocalFile(item->text()));
    m_player->play();
    //QMediaMetaData data = m_player->metaData();
    //ui->track_name->setText(m_player->metaData().stringValue(QMediaMetaData::Author));
    //qDebug() << data.stringValue(QMediaMetaData::Title);
    //qDebug() << m_player->metaData().stringValue(QMediaMetaData::Author);
}


void MainWindow::on_muteBtn_clicked()
{
    m_player->stop();
    QTableWidgetItem *item = ui->tableWidget->item(++current_track, 0);
    m_player->setSource(QUrl::fromLocalFile(item->text()));
    m_player->play();
    //QMediaMetaData data = m_player->metaData();
    //ui->track_name->setText(m_player->metaData().stringValue(QMediaMetaData::Author));
    //qDebug() << data.stringValue(QMediaMetaData::Title);
    //qDebug() << data.stringValue(data.Author);
}

void MainWindow::on_trackSlider_sliderMoved(int position)
{
    m_player->setPosition(position);
}


void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    m_audioOutput->setVolume(position);
}

void MainWindow::on_positionChanged(quint64 position)
{
    ui->trackSlider->setValue(position);
    //ui->volumeSlider->setValue(position);
}

void MainWindow::on_durationChanged(quint64 position)
{
    ui->volumeSlider->setValue(position);
    //ui->trackSlider->setValue(position);
}

void MainWindow::on_nextTrackBtn_clicked()
{

}


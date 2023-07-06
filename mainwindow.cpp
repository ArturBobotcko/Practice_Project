#include "mainwindow.h"
#include "StyleHelper.h"
#include "databasehandler.h"
#include "selectplaylist.h"
#include <QMessageBox>

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
    mix = false;
    pause_position = 0;

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->volumeSlider->setValue(50);

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
    connect(ui->up_button, &QPushButton::clicked, this, &MainWindow::up_buttonClicked);
    connect(ui->down_button, &QPushButton::clicked, this, &MainWindow::down_buttonClicked);

    connect(ui->trackSlider, &QSlider::sliderMoved, this, &MainWindow::trackSlider_sliderMoved);
    connect(ui->volumeSlider, &QSlider::sliderMoved, this, &MainWindow::volumeSlider_sliderMoved);
    connect(ui->trackSlider, &QSlider::valueChanged, this, &MainWindow::trackSlider_valueChanged);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::volumeSlider_valueChanged);
    connect(ui->playlist_list, &QTableWidget::cellDoubleClicked, this, &MainWindow::playlistDoubleClicked);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, &QWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    ui->tableWidget->selectRow(0);

    ui->mixButton->setIconSize(QSize(25, 25));
    ui->repeatButton->setIconSize(QSize(25, 25));
    QWidget::setWindowTitle("Spotify enjoyers");
    QWidget::setWindowIcon(QIcon(":/new/prefix1/resources/play.svg"));
}

void MainWindow::playTrack()
{
    m_player->stop();
    if (mix) {
        int row = QRandomGenerator::global()->bounded(0, ui->tableWidget->rowCount());
        ui->tableWidget->selectRow(row);
    } else {
        ui->tableWidget->selectRow(ui->tableWidget->currentRow());
    }
    QTableWidgetItem *item = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
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
    for (int row = 0; row < track->rowCount(); ++row)
    {
        QString path = track->record(row).value("path").toString();
        QString track_name = track->record(row).value("track_name").toString();
        QString author = track->record(row).value("author").toString();
        QString duration = track->record(row).value("duration").toString();
        QTableWidgetItem* item = new QTableWidgetItem(path);
        QTableWidgetItem* item2 = new QTableWidgetItem(track_name);
        QTableWidgetItem* item3 = new QTableWidgetItem(author);
        QTableWidgetItem* item4 = new QTableWidgetItem(duration);
        ui->tableWidget->setItem(row, 0, item);
        ui->tableWidget->setItem(row, 1, item2);
        ui->tableWidget->setItem(row, 2, item3);
        ui->tableWidget->setItem(row, 3, item4);
    }
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

void MainWindow::up_buttonClicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow > 0 && currentRow < ui->tableWidget->rowCount()) {
        QList<QTableWidgetItem*> selectedRow;
        for (int column = 0; column < ui->tableWidget->columnCount(); column++) {
            selectedRow.append(ui->tableWidget->item(currentRow, column)->clone());
        }
        QList<QTableWidgetItem*> aboveRow;
        for (int column = 0; column < ui->tableWidget->columnCount(); column++) {
            aboveRow.append(ui->tableWidget->item(currentRow - 1, column)->clone());
        }
        for (int column = 0; column < ui->tableWidget->columnCount(); column++) {
            ui->tableWidget->setItem(currentRow - 1, column, selectedRow.at(column));
            ui->tableWidget->setItem(currentRow, column, aboveRow.at(column));
        }
        ui->tableWidget->setCurrentCell(currentRow - 1, ui->tableWidget->currentColumn());
    }
}

void MainWindow::down_buttonClicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow >= 0 && currentRow < ui->tableWidget->rowCount() - 1) {
        QList<QTableWidgetItem*> selectedRow;
        for (int column = 0; column < ui->tableWidget->columnCount(); column++) {
            selectedRow.append(ui->tableWidget->item(currentRow, column)->clone());
        }
        QList<QTableWidgetItem*> aboveRow;
        for (int column = 0; column < ui->tableWidget->columnCount(); column++) {
            aboveRow.append(ui->tableWidget->item(currentRow + 1, column)->clone());
        }
        for (int column = 0; column < ui->tableWidget->columnCount(); column++) {
            ui->tableWidget->setItem(currentRow + 1, column, selectedRow.at(column));
            ui->tableWidget->setItem(currentRow, column, aboveRow.at(column));
        }
        ui->tableWidget->setCurrentCell(currentRow + 1, ui->tableWidget->currentColumn());
    }
}

void MainWindow::showContextMenu()
{
    QItemSelectionModel* selectionModel = ui->tableWidget->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();
    if (!selectedRows.isEmpty()) {
        QMenu menu(this);

        QAction* action1 = new QAction("Добавить в плейлист", this);
        connect(action1, &QAction::triggered, [this, selectedRows]() {
            addTrackToPlaylist(1, selectedRows);
        });

        QAction* action2 = new QAction("Удалить трек", this);
        connect(action2, &QAction::triggered, [this, selectedRows]() {
            deleteTrack(2, selectedRows);
        });

        menu.addAction(action1);
        menu.addAction(action2);


        menu.exec(QCursor::pos());
    }
}

void MainWindow::addTrackToPlaylist(int actionId, const QModelIndexList& selectedRows)
{
    QStringList rowValues;
    for (const QModelIndex& index : selectedRows) {
        QStringList columnValues;
        for (int column = 0; column < ui->tableWidget->columnCount(); ++column) {
            QTableWidgetItem* item = ui->tableWidget->item(index.row(), column);
            if (item) {
                QString value = item->text();
                columnValues.append(value);
            }
        }
        rowValues.append(columnValues);
    }

    if (!selectPlaylistDialog)
    {
        selectPlaylistDialog = new selectPlaylist(this);

    }
    selectPlaylistDialog->setRowValues(rowValues);
    selectPlaylistDialog->fullComboBox();
    selectPlaylistDialog->show();

    qDebug() << "Выполнено действие" << actionId << "для строк:" << rowValues;
}

void MainWindow::deleteTrack(int actionId, const QModelIndexList &selectedRows)
{
    if (selectedRows.isEmpty()) {
        return;
    }
    // Отображение окна предупреждения
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение удаления", "Вы уверены, что хотите удалить выбранный трек?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }
    DataBaseHandler::instance().deleteTrack(selectedRows);
    insertTracks();
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
    playlist->insertTracks();
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        pause_position = m_player->position();
        m_player->stop();
    }
    hide();
    playlist->show();
}

void MainWindow::addTracks_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    QStringList selected_files = dialog.getOpenFileNames();
    foreach(const QString& file, selected_files) {
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
    QMediaPlayer* player = qobject_cast<QMediaPlayer*>(sender());
    if (player) {
        QMediaMetaData data = player->metaData();

        QString filePath = player->source().path();
        QString title = data.stringValue(QMediaMetaData::Title);
        QString artist = data.stringValue(QMediaMetaData::ContributingArtist);
        QString duration = data.stringValue(QMediaMetaData::Duration);

        // Проверка наличия песни в таблице
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
            QTableWidgetItem* filePathItem = ui->tableWidget->item(row, 0);
            QTableWidgetItem* titleItem = ui->tableWidget->item(row, 1);
            QTableWidgetItem* artistItem = ui->tableWidget->item(row, 2);

            if (filePathItem && titleItem && artistItem) {
                if (filePathItem->text() == filePath && titleItem->text() == title && artistItem->text() == artist) {
                    return; // Песня уже существует в таблице, выходим из функции
                }
            }
        }

        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(player->source().path()));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(data.stringValue(QMediaMetaData::Title)));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(data.stringValue(QMediaMetaData::ContributingArtist)));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(data.stringValue(QMediaMetaData::Duration)));

        DataBaseHandler::instance().addTrack(player->source().path(), data.stringValue(QMediaMetaData::Title), data.stringValue(QMediaMetaData::ContributingArtist), data.stringValue(QMediaMetaData::Duration));
    }
}

void MainWindow::stopTrackBtn_clicked()
{
    m_player->stop();
    ui->trackSlider->setValue(0);
    pause_position = 0;
}

void MainWindow::prevTrackBtn_clicked()
{
    if (ui->tableWidget->currentRow() == 0) {
        ui->tableWidget->setCurrentCell(ui->tableWidget->rowCount() - 1, 0);
    } else {
        ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow() - 1, 0);
    }
    playTrack();
}

void MainWindow::nextTrackBtn_clicked()
{
    if (ui->tableWidget->currentRow() == ui->tableWidget->rowCount() - 1) {
        ui->tableWidget->setCurrentCell(0, 0);
    } else {
        ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow() + 1, 0);
    }
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
    if (mix) {
        mix = false;
    } else {
        mix = true;
    }
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
    QTime time = QTime::fromMSecsSinceStartOfDay(value);
    ui->trackTimer->setText(time.toString("mm:ss:zzz"));
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
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение удаления", "Вы уверены, что хотите удалить выбранный плейлист?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }
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

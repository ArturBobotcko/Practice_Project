#include "playlistwindow.h"
#include "ui_playlistwindow.h"
#include "StyleHelper.h"
#include "databasehandler.h"

PlaylistWindow::PlaylistWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaylistWindow)
{
    ui->setupUi(this);
    setIntefaceStyle();
    mainWindow = parent;

    ui->playlistTracks->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->playlistTracks->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->playlistTracks->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->playlistTracks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->volumeSlider->setValue(50);

    connect(m_player.get_player(), &QMediaPlayer::positionChanged, this, &PlaylistWindow::trackSlider_valueChanged);
    connect(m_player.get_player(), &QMediaPlayer::durationChanged, this, &PlaylistWindow::setDuration);
    connect(m_player.get_player(), &QMediaPlayer::playbackStateChanged, this, &PlaylistWindow::changedPlaybackState);

    connect(ui->playlistTracks, &QTableWidget::cellDoubleClicked, this, &PlaylistWindow::cellDoubleClicked);
    connect(ui->playBtn, &QPushButton::clicked, this, &PlaylistWindow::playBtn_clicked);
    connect(ui->stopTrackBtn, &QPushButton::clicked, this, &PlaylistWindow::stopTrackBtn_clicked);
    connect(ui->prevTrackBtn, &QPushButton::clicked, this, &PlaylistWindow::prevTrackBtn_clicked);
    connect(ui->muteBtn, &QPushButton::clicked, this, &PlaylistWindow::muteBtn_clicked);
    connect(ui->nextTrackBtn, &QPushButton::clicked, this, &PlaylistWindow::nextTrackBtn_clicked);

    connect(ui->repeatButton, &QPushButton::clicked, this, &PlaylistWindow::repeatBtn_clicked);
    connect(ui->mixButton, &QPushButton::clicked, this, &PlaylistWindow::mixBtn_clicked);
    connect(ui->up_button, &QPushButton::clicked, this, &PlaylistWindow::up_buttonClicked);
    connect(ui->down_button, &QPushButton::clicked, this, &PlaylistWindow::down_buttonClicked);

    connect(ui->trackSlider, &QSlider::sliderMoved, this, &PlaylistWindow::trackSlider_sliderMoved);
    connect(ui->trackSlider, &QSlider::valueChanged, this, &PlaylistWindow::trackSlider_valueChanged);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &PlaylistWindow::volumeSlider_sliderMoved);
    ui->playlistTracks->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->playlistTracks, &QWidget::customContextMenuRequested, this, &PlaylistWindow::showContextMenu);
    ui->playlistTracks->selectRow(0);

    ui->mixButton->setIconSize(QSize(25, 25));
    ui->repeatButton->setIconSize(QSize(25, 25));
    QWidget::setWindowTitle("Spotify enjoyers");
    QWidget::setWindowIcon(QIcon(":/new/prefix1/resources/play.svg"));
}

void PlaylistWindow::closeEvent(QCloseEvent* event)
{
    if (m_player.get_playbackState() == QMediaPlayer::PlayingState) {
        m_player.set_pause_position(m_player.get_position());
        m_player.stop_track();
    }
    mainWindow->show();
    //mainWindow->closeEvent(event);
}

PlaylistWindow::~PlaylistWindow()
{
    delete ui;
}

void PlaylistWindow::setPlaylistName(const QString &playlistName)
{
    ui->playlistName->setText(playlistName);
}

void PlaylistWindow::setIntefaceStyle()
{
    StyleHelper::setAllButtonsStyle(ui);
    StyleHelper::setAllSlidersStyle(ui);
}

void PlaylistWindow::insertTracks()
{
    QSqlQuery query;
    QString playlist_name = ui->playlistName->text();
    QString selectQuery = QString("SELECT id FROM Playlists WHERE playlist_name = '%1'").arg(playlist_name);
    if(!query.exec(selectQuery)) {
        qDebug() << "Unable to select from 'Playlists' table: " + query.lastError().text();
        return;
    }
    if (query.next()) {
        int id_playlist = query.value(0).toInt();
        QSqlQueryModel* query = new QSqlQueryModel();

        QString selectTracksQuery = QString("SELECT * FROM AllTracks WHERE id_playlist = %1").arg(id_playlist);
        query->setQuery(selectTracksQuery);
        qDebug() << query->rowCount();
        // Установка количества строк в таблице playlistTracks
        ui->playlistTracks->setRowCount(query->rowCount());
        ui->playlistTracks->setColumnCount(4);
        for (int row = 0; row < query->rowCount(); ++row) {
            for (int column = 1; column < 5; ++column) {
                QModelIndex index = query->index(row, column);
                QVariant value = query->data(index);

                if (value.isValid()) {
                    QString cellValue = value.toString();
                    qDebug() << row << ": " << cellValue;
                    // Установка значений в каждую колонку
                    QTableWidgetItem *item = new QTableWidgetItem(cellValue);
                    ui->playlistTracks->setItem(row, column - 1, item);
                } else {
                    qDebug() << "Invalid value at row" << row << "column" << column;
                }
            }
        }
    } else {
        qDebug() << "No valid record found for the given condition.";
        return;
    }
}

void PlaylistWindow::playTrack()
{
    m_player.stop_track();
    if (m_player.is_mixed()) {
        int row = QRandomGenerator::global()->bounded(0, ui->playlistTracks->rowCount());
        ui->playlistTracks->selectRow(row);
    } else {
        ui->playlistTracks->selectRow(ui->playlistTracks->currentRow());
    }
    QTableWidgetItem *item = ui->playlistTracks->item(ui->playlistTracks->currentRow(), 0);
    if (!item) {
        return;
    }
    m_player.set_source(QUrl::fromLocalFile(item->text()));
    m_player.play_track();
}

void PlaylistWindow::cellDoubleClicked(int iRow, int iColumn)
{
    playTrack();
}
// Функция вставки списка плейлистов в виджет таблицы

void PlaylistWindow::playBtn_clicked()
{
    if (m_player.get_playbackState() == QMediaPlayer::PlayingState) {
        m_player.set_pause_position(m_player.get_position());
        m_player.stop_track();
    } else {
        m_player.set_position(ui->trackSlider->value());
        playTrack();
    }
}

void PlaylistWindow::up_buttonClicked()
{
    int currentRow = ui->playlistTracks->currentRow();
    if (currentRow > 0 && currentRow < ui->playlistTracks->rowCount()) {
        QList<QTableWidgetItem*> selectedRow;
        for (int column = 0; column < ui->playlistTracks->columnCount(); column++) {
            selectedRow.append(ui->playlistTracks->item(currentRow, column)->clone());
        }
        QList<QTableWidgetItem*> aboveRow;
        for (int column = 0; column < ui->playlistTracks->columnCount(); column++) {
            aboveRow.append(ui->playlistTracks->item(currentRow - 1, column)->clone());
        }
        for (int column = 0; column < ui->playlistTracks->columnCount(); column++) {
            ui->playlistTracks->setItem(currentRow - 1, column, selectedRow.at(column));
            ui->playlistTracks->setItem(currentRow, column, aboveRow.at(column));
        }
        ui->playlistTracks->setCurrentCell(currentRow - 1, ui->playlistTracks->currentColumn());
    }
}

void PlaylistWindow::down_buttonClicked()
{
    int currentRow = ui->playlistTracks->currentRow();
    if (currentRow >= 0 && currentRow < ui->playlistTracks->rowCount() - 1) {
        QList<QTableWidgetItem*> selectedRow;
        for (int column = 0; column < ui->playlistTracks->columnCount(); column++) {
            selectedRow.append(ui->playlistTracks->item(currentRow, column)->clone());
        }
        QList<QTableWidgetItem*> aboveRow;
        for (int column = 0; column < ui->playlistTracks->columnCount(); column++) {
            aboveRow.append(ui->playlistTracks->item(currentRow + 1, column)->clone());
        }
        for (int column = 0; column < ui->playlistTracks->columnCount(); column++) {
            ui->playlistTracks->setItem(currentRow + 1, column, selectedRow.at(column));
            ui->playlistTracks->setItem(currentRow, column, aboveRow.at(column));
        }
        ui->playlistTracks->setCurrentCell(currentRow + 1, ui->playlistTracks->currentColumn());
    }
}

void PlaylistWindow::showContextMenu()
{
    QItemSelectionModel* selectionModel = ui->playlistTracks->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();
    if (!selectedRows.isEmpty()) {
        QMenu menu(this);

        QAction* action2 = new QAction("Удалить трек", this);
        connect(action2, &QAction::triggered, [this, selectedRows]() {
            deleteTrack(2, selectedRows);
        });

        //menu.addAction(action1);
        menu.addAction(action2);


        menu.exec(QCursor::pos());
    }
}

void PlaylistWindow::deleteTrack(int actionId, const QModelIndexList &selectedRows)
{
    QStringList rowValues;
    for (const QModelIndex& index : selectedRows) {
        QStringList columnValues;
        for (int column = 0; column < ui->playlistTracks->columnCount(); ++column) {
            QTableWidgetItem* item = ui->playlistTracks->item(index.row(), column);
            if (item) {
                QString value = item->text();
                columnValues.append(value);
            }
        }
        rowValues.append(columnValues);
    }
   // Сделать update на изменение поля айди плейлиста в нуль и форму заново выбрать короче
    int currentPlaylistId; // Ваш способ получения текущего id плейлиста
    qDebug() << selectedRows.at(1);
    // Выполнение SQL-запроса для обновления полей id_playlist в таблице AllTracks
    QSqlQuery query;
    query.prepare("SELECT id FROM Playlists WHERE name = :playlistName");
    query.bindValue(":playlistName", ui->playlistName->text());
    if (query.exec() && query.next()) {
        currentPlaylistId = query.value(0).toInt(); // Получение значения идентификатора плейлиста из первого столбца
    }
    query.prepare("UPDATE AllTracks SET id_playlist = NULL WHERE id_playlist = :playlistId AND path = :path AND track_name = :track_name AND author = :author AND duration = :duration");
    query.bindValue(":playlistId", currentPlaylistId);
    query.bindValue(":path", rowValues.at(0));
    query.bindValue(":track_name", rowValues.at(1));
    query.bindValue(":author", rowValues.at(2));
    query.bindValue(":duration", rowValues.at(3));
    query.exec();

    insertTracks();
}

void PlaylistWindow::changedPlaybackState()
{
    QIcon icon;
    if (m_player.get_playbackState() == QMediaPlayer::PlayingState) {
        icon.addFile(QString::fromUtf8(":/new/prefix1/resources/pause.svg"), QSize(), QIcon::Normal, QIcon::Off);
    } else {
        icon.addFile(QString::fromUtf8(":/new/prefix1/resources/play.svg"), QSize(), QIcon::Normal, QIcon::Off);
        ui->trackSlider->setValue(m_player.get_pause_position());
    }
    ui->playBtn->setIcon(icon);
    ui->playBtn->setIconSize(QSize(50, 50));
    ui->playBtn->setFlat(false);
}

void PlaylistWindow::stopTrackBtn_clicked()
{
    ui->trackSlider->setValue(0);
    m_player.set_pause_position(0);
    m_player.stop_track();
}

void PlaylistWindow::prevTrackBtn_clicked()
{
    if (ui->playlistTracks->currentRow() == 0) {
        ui->playlistTracks->setCurrentCell(ui->playlistTracks->rowCount() - 1, 0);
    } else {
        ui->playlistTracks->setCurrentCell(ui->playlistTracks->currentRow() - 1, 0);
    }
    playTrack();
}

void PlaylistWindow::nextTrackBtn_clicked()
{
    if (ui->playlistTracks->currentRow() == ui->playlistTracks->rowCount() - 1) {
        ui->playlistTracks->setCurrentCell(0, 0);
    } else {
        ui->playlistTracks->setCurrentCell(ui->playlistTracks->currentRow() + 1, 0);
    }
    playTrack();
}

void PlaylistWindow::repeatBtn_clicked()
{
    if (m_player.get_player()->loops() == QMediaPlayer::Once) {
        ui->repeatButton->setIcon(QIcon(":/new/prefix1/resources/repeat.svg"));
        m_player.get_player()->setLoops(QMediaPlayer::Infinite);
    } else {
        ui->repeatButton->setIcon(QIcon(":/new/prefix1/resources/no-repeat.svg"));
        m_player.get_player()->setLoops(QMediaPlayer::Once);
    }
}

void PlaylistWindow::mixBtn_clicked()
{
    m_player.set_mix();
}

void PlaylistWindow::setDuration()
{
    ui->trackSlider->setMaximum(m_player.get_duration());
}

void PlaylistWindow::muteBtn_clicked()
{
    if (!m_player.is_muted()) {
        m_player.set_current_volume(ui->volumeSlider->value());
        ui->muteBtn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/resources/muted-volume.svg")));
        m_player.set_volume(0);
        ui->volumeSlider->setValue(0);
        m_player.set_mute();
    }
    else {
        m_player.set_volume(m_player.get_current_volume()/100.0f);
        ui->volumeSlider->setValue(m_player.get_current_volume());
        m_player.set_mute();
    }
}

void PlaylistWindow::trackSlider_sliderMoved(int position)
{
    m_player.set_position(position);
}

void PlaylistWindow::trackSlider_valueChanged(int value)
{
    QTime time = QTime::fromMSecsSinceStartOfDay(value);
    ui->trackTimer->setText(time.toString("mm:ss:zzz"));
    ui->trackSlider->setValue(value);
}

void PlaylistWindow::volumeSlider_sliderMoved(int value)
{
    if (value <= 100 && value >= 75) {
        ui->muteBtn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/resources/high-volume.svg")));
    }
    else if (value < 75 && value >= 25) {
        ui->muteBtn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/resources/mid-volume.svg")));
    }
    else if (value < 25 && value != 0) {
        ui->muteBtn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/resources/low-volume.svg")));
    } else {
        ui->muteBtn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/resources/muted-volume.svg")));
    }
    m_player.set_volume(value/100.0f);
}

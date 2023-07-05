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
    if(!query.exec(selectQuery))
    {
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

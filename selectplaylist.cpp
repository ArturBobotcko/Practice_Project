#include "selectplaylist.h"
#include "ui_selectplaylist.h"
#include "databasehandler.h"

selectPlaylist::selectPlaylist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectPlaylist)
{
    ui->setupUi(this);
}

selectPlaylist::~selectPlaylist()
{
    delete ui;
}

void selectPlaylist::fullComboBox()
{
    QSqlQuery query;
    QString selectQuery = "SELECT playlist_name FROM Playlists";
    if(!query.exec(selectQuery))
    {
        qDebug() << "Unable to select from 'Playlists' table: " + query.lastError().text();
        return;
    }

    QStringList result;
    while (query.next()) {
        QString value = query.value(0).toString();  // Пример получения значения из первого столбца
        result.append(value);
    }
    ui->comboBox->clear();
    ui->comboBox->addItems(result);
}

void selectPlaylist::setRowValues(const QStringList &newRowValues)
{
    rowValues = newRowValues;
}

void selectPlaylist::on_pushButton_clicked()
{
    QString selectedPlaylist = ui->comboBox->currentText();
    QSqlQuery query;
    qDebug() <<selectedPlaylist;

    QString selectQuery = QString("SELECT id FROM Playlists WHERE playlist_name = '%1'").arg(selectedPlaylist);
    if(!query.exec(selectQuery))
    {
        qDebug() << "Unable to select from 'Playlists' table: " + query.lastError().text();
        return;
    }

    qDebug() << rowValues.count();
    qDebug() << "test: " << rowValues.at(1);

    if (query.next()) {
        int id_playlist = query.value(0).toInt();
        QString updateQuery = QString("UPDATE AllTracks SET id_playlist = %1 WHERE path = '%2' AND track_name = '%3' AND author = '%4' AND duration = '%5'")
                                  .arg(id_playlist)
                                  .arg(rowValues.at(0))
                                  .arg(rowValues.at(1))
                                  .arg(rowValues.at(2))
                                  .arg(rowValues.at(3));
        if (!query.exec(updateQuery)) {
            qDebug() << "Unable to update 'AllTracks' table: " + query.lastError().text();
            return;
        }
    } else {
        qDebug() << "No valid record found for the given condition.";
        return;
    }

    close();
}


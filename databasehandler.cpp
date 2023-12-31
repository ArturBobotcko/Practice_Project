#include "databasehandler.h"
#include "mainwindow.h"

DataBaseHandler::DataBaseHandler() {}

DataBaseHandler& DataBaseHandler::instance()
{
    static DataBaseHandler instance;
    return instance;
}

void DataBaseHandler::connectToDataBase(QSqlDatabase& db)
{
    if (!db.open()) {
        qDebug() << "Unable to connect to database: " << db.lastError().text();
        return;
    }
    qDebug() << "Database connected!";
}

void DataBaseHandler::createDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString projPath = QDir::currentPath();
    QString fullPath = projPath + "/db_playlist.sqlite";
    qDebug() << fullPath;
    db.setDatabaseName(fullPath);
    connectToDataBase(db);
    QSqlQuery query;

    if (!db.tables().contains("Playlists")) {
        query.exec("CREATE TABLE Playlists"
                   "(id integer primary key,"
                   "playlist_name varchar(40)"
                   ")");
        if (query.lastError().isValid()) {
            qDebug() << "Unable to create table 'Playlists': " << query.lastError().text();
        }
        else {
            qDebug() << "Table 'Playlists' created.";
        }
    }
    db.close();
}

bool DataBaseHandler::addPlaylist(const QString& playlist_name)
{
    QSqlQuery query;
    QString selectQuery = QString("SELECT playlist_name FROM Playlists WHERE playlist_name = '%1'").arg(playlist_name);
    QString query_text = QString("INSERT OR IGNORE INTO Playlists(playlist_name) VALUES ('%1')").arg(playlist_name);
    connectToDataBase(db);
    query.exec(selectQuery);
    if (query.next()) {
        qDebug() << "Playlist with name " << playlist_name << " already exists.";
        return false;
    }
    else {
        if (!query.exec(query_text)) {
            qDebug() << "Unable to insert into 'Playlists' table: " + query.lastError().text();
            return false;
        }
        qDebug() << "Playlist " << playlist_name << " added!";
    }
    db.close();
    return true;
}

void DataBaseHandler::deletePlaylist(const QString &playlist_name)
{
    if (playlist_name != "")
    {
        QSqlQuery query;
        QString deleteQuery = QString("DELETE FROM Playlists WHERE playlist_name = '%1'").arg(playlist_name);
        if(!query.exec(deleteQuery)) {
            qDebug() << "Unable to delete playlist from 'Playlists' table: " + query.lastError().text();
        }
        else {
            qDebug() << "Playlist " << playlist_name << " deleted!";
        }
    }
    else {
        qDebug() << "Unable to delete playlist with empty name";
    }
}

QSqlQueryModel* DataBaseHandler::getPlaylists()
{
    connectToDataBase(db);
    QSqlQueryModel* query = new QSqlQueryModel();
    query->setQuery("SELECT * FROM Playlists");
    if(query->lastError().isValid()) {
        qDebug() << "Unable to select from 'Playlists' table: " + query->lastError().text();
    }
    return query;
}

bool DataBaseHandler::addTrack(const QString& path_value, const QString& track_name,
                               const QString& author_value, const QString& duration_value)
{
    connectToDataBase(db);
    QSqlQuery query;
    qDebug() << "TEST: " << db.tables().contains("AllTracks");
    if (!db.tables().contains("AllTracks")) {
        qDebug() << "There is no table 'AllTracks'. Creating...";
        if (!query.exec("CREATE TABLE AllTracks"
                   "(id integer primary key,"
                   "path varchar(100),"
                   "track_name varchar(40),"
                   "author varchar(50),"
                   "duration integer,"
                   "id_playlist integer"
                       ")"))
         qDebug() << query.lastError().text();
    }

    QString path = path_value;
    QString artist = author_value;
    path.replace("'","''");
    artist.replace("'","''");
    qDebug() << path;

    QString selectQuery = QString("SELECT * FROM AllTracks WHERE track_name='%1' AND author='%2' AND duration='%3'")
            .arg(track_name, artist, duration_value);
    QString query_text = QString("INSERT OR IGNORE INTO AllTracks(path, track_name, author, duration) "
                                 "VALUES ('%1', '%2', '%3', '%4')").arg(path, track_name, artist, duration_value);
    query.exec(selectQuery);
    if (query.next()) {
        qDebug() << "This track already added!";
        return false;
    }
    if (!query.exec(query_text)) {
        qDebug() << "Unable to insert into 'AllTracks"
                    "' table: " + query.lastError().text();
        return false;
    }
    if (query.next()) {
        qDebug() << "Track with name " << track_name << " already exists.";
        return false;
    }
    else {
        qDebug() << "Track " << track_name << " added!";
    }
    db.close();
    return true;
}

QSqlQueryModel* DataBaseHandler::getTracks()
{
    connectToDataBase(db);
    QSqlQueryModel* query = new QSqlQueryModel();
    query->setQuery("SELECT * FROM AllTracks");
    if(query->lastError().isValid()) {
        qDebug() << "Unable to select from 'AllTracks' table: " + query->lastError().text();
    }
    return query;
}

void DataBaseHandler::deleteTrack(const QModelIndexList &selectedRows)
{
    QString deleteQuery = "DELETE FROM AllTracks WHERE path = :path AND track_name = :track_name AND author = :author AND duration = :duration";
    QSqlQuery query;
    if (!query.prepare(deleteQuery)) {
        qDebug() << "Unable to prepare delete query: " + query.lastError().text();
        return;
    }

    foreach (const QModelIndex& index, selectedRows) {
        QString path = index.sibling(index.row(), 0).data().toString();
        QString trackName = index.sibling(index.row(), 1).data().toString();
        QString author = index.sibling(index.row(), 2).data().toString();
        QString duration = index.sibling(index.row(), 3).data().toString();

        query.bindValue(":path", path);
        query.bindValue(":track_name", trackName);
        query.bindValue(":author", author);
        query.bindValue(":duration", duration);

        if (!query.exec()) {
            qDebug() << "Unable to delete from 'AllTracks' table: " + query.lastError().text();
            return;
        }
    }
}

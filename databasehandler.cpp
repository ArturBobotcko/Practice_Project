#include "databasehandler.h"
#include "mainwindow.h"

DataBaseHandler::DataBaseHandler() {}

// Функция возвращает ссылку на единственный экземпляр класса
DataBaseHandler& DataBaseHandler::instance()
{
    static DataBaseHandler instance;
    return instance;
}

// Функция присоединения к базе данных
void DataBaseHandler::connectToDataBase(QSqlDatabase db)
{
    if(!db.open())
    {
        qDebug() << "Unable to connect to database: " << db.lastError().text();
        return;
    }
    qDebug() << "Database connected!";
}

// Функция создает базу данных
void DataBaseHandler::createDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString projPath = QDir::currentPath();
    QString fullPath = projPath + "/db_playlist.sqlite";
    qDebug() << fullPath;
    db.setDatabaseName(fullPath);
    connectToDataBase(db);
    QSqlQuery query;
    // Если таблицы Playlists нет, создаем её
    if (!db.tables().contains("Playlists"))
    {
        query.exec("CREATE TABLE Playlists"
                   "(id integer primary key,"
                   "playlist_name varchar(40)"
                   ")");
        if (query.lastError().isValid())
        {
            qDebug() << "Unable to create table 'Playlists': " << query.lastError().text();
        }
        else
        {
            qDebug() << "Table 'Playlists' created.";
        }
    }
    db.close();
}

// Функция добавления плейлиста в таблицу
// true - плейлист был добавлен, false - не был
bool DataBaseHandler::addPlaylist(const QString& playlist_name)
{
    QSqlQuery query;
    QString selectQuery = QString("SELECT playlist_name FROM Playlists WHERE playlist_name = '%1'").arg(playlist_name);
    QString query_text = QString("INSERT OR IGNORE INTO Playlists(playlist_name) VALUES ('%1')").arg(playlist_name);
    connectToDataBase(db);
    query.exec(selectQuery);
    if (query.next())
    {
        qDebug() << "Playlist with name " << playlist_name << " already exists.";
        return false;
    }
    else
    {
        if(!query.exec(query_text))
        {
            qDebug() << "Unable to insert into 'Playlists' table: " + query.lastError().text();
            return false;
        }
        qDebug() << "Playlist " << playlist_name << " added!";
        return true;
    }
    db.close();
}

// Функция удаления записи из таблицы
void DataBaseHandler::deletePlaylist(const QString &playlist_name)
{
    if (playlist_name != "")
    {
        QSqlQuery query;
        QString deleteQuery = QString("DELETE FROM Playlists WHERE playlist_name = '%1'").arg(playlist_name);
        if(!query.exec(deleteQuery))
        {
            qDebug() << "Unable to delete playlist from 'Playlists' table: " + query.lastError().text();
        }
        else
        {
            qDebug() << "Playlist " << playlist_name << " deleted!";
        }
    }
    else
    {
        qDebug() << "Unable to delete playlist with empty name";
    }
}

// Функция получения списка всех плейлистов из БД
QSqlQueryModel* DataBaseHandler::getPlaylists()
{
    connectToDataBase(db);
    QSqlQueryModel* query = new QSqlQueryModel();
    query->setQuery("SELECT * FROM Playlists");
    if(query->lastError().isValid())
    {
        qDebug() << "Unable to select from 'Playlists' table: " + query->lastError().text();
    }
    return query;
}

bool DataBaseHandler::addTrack(const QString& path_value, const QString& track_name, const QString& author_value, const QString& duration_value)
{
    connectToDataBase(db);
    QSqlQuery query;
   // QString track_name, author_value, duration_value;
    //не туда ывводятся названия
    QString selectQuery = QString("SELECT (path, track_name, author, duration) FROM AllTracks WHERE path='%1' AND track_name='%2' AND author='%3' AND duration='%4'").arg(path_value).arg(track_name).arg(author_value).arg(duration_value);

    QString query_text = QString("INSERT OR IGNORE INTO AllTracks(path, track_name, author, duration) VALUES ('%1', '%2', '%3', '%4')").arg(path_value).arg(track_name).arg(author_value).arg(duration_value);


    query.exec(selectQuery);
    if (!db.tables().contains("AllTracks"))
    {
        query.exec("CREATE TABLE AllTracks"
                   "(id integer primary key,"
                   "path varchar(100),"
                   "track_name varchar(40),"
                   "author varchar(50),"
                   "duration integer"
                   ")");
        if (query.lastError().isValid())
        {
            qDebug() << "Unable to create table 'AllTracks': " << query.lastError().text();
        }
        else
        {
            qDebug() << "Table 'AllTracks' created.";
        }
    }
    if (query.next())
    {
        qDebug() << "Track with name " << path_value << " already exists.";
        return false;
    }
    else
    {
        if(!query.exec(query_text))
        {
            qDebug() << "Unable to insert into 'Tracks' table: " + query.lastError().text();
            return false;
        }
        qDebug() << "Track " << path_value << " added!";
        return true;
    }
    db.close();
}

// Функция получения списка всех треков из БД
QSqlQueryModel* DataBaseHandler::getTracks()
{
    connectToDataBase(db);
    QSqlQueryModel* query = new QSqlQueryModel();
    query->setQuery("SELECT * FROM AllTracks");
    if(query->lastError().isValid())
    {
        qDebug() << "Unable to select from 'AllTracks' table: " + query->lastError().text();
    }
    return query;
}

#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QtSql>
#include <QList>
#include <QVariant>
#include <QSqlRecord>
#include <QTableWidget>
#include <QTableWidgetItem>

class DataBaseHandler
{
private:
    DataBaseHandler();
    DataBaseHandler(const DataBaseHandler&) = delete;
    DataBaseHandler& operator=(const DataBaseHandler&) = delete;
    QSqlDatabase db;
    void connectToDataBase(QSqlDatabase db);
public:
    static DataBaseHandler& instance();
    void createDataBase();
    bool addPlaylist(const QString& playlist_name);
    void deletePlaylist(const QString& playlist_name);
    QSqlQueryModel* getPlaylists();
    bool addTrack(const QString& path_value, const QString& track_name, const QString& author_value, const QString& duration_value);
    QSqlQueryModel* getTracks();
    void deleteTrack(const QModelIndexList &selectedRows);
};

#endif // DATABASEHANDLER_H

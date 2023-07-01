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
public:
    static DataBaseHandler& instance();
    void createDataBase();
    void connectToDataBase(QSqlDatabase db);
    bool addPlaylist(const QString& playlist_name);
   // void refreshPlaylists(QSqlDatabase db);
};

#endif // DATABASEHANDLER_H

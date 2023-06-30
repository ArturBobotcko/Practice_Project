#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QtSql>

class DataBaseHandler
{
public:
    DataBaseHandler();
    static QSqlDatabase createDataBase();
    static void connectToDataBase(QSqlDatabase db);
    static void createPlaylistTable();
    static void createTable();
};

#endif // DATABASEHANDLER_H

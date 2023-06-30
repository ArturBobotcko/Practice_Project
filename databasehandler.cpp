#include "databasehandler.h"

DataBaseHandler::DataBaseHandler()
{

}

QSqlDatabase DataBaseHandler::createDataBase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/databases/MainDataBase");
    return db;
}

void DataBaseHandler::connectToDataBase(QSqlDatabase db)
{
    if(!db.open())
    {
        qDebug() << "Unable to connect to database: " << db.lastError().text();
        return;
    }
}

#ifndef ADD_PLAYLIST_H
#define ADD_PLAYLIST_H

#include "databasehandler.h"

#include <QMainWindow>
#include <QDialog>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>


namespace Ui {
class add_playlist;
}

class add_playlist : public QDialog
{
    Q_OBJECT

public:
    explicit add_playlist(QWidget *parent = nullptr);
    ~add_playlist();

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::add_playlist *ui;
    QSqlTableModel *model;
};

#endif // ADD_PLAYLIST_H

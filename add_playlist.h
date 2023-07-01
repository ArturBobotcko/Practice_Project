#ifndef ADD_PLAYLIST_H
#define ADD_PLAYLIST_H

#include "databasehandler.h"

#include <QDialog>

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
};

#endif // ADD_PLAYLIST_H

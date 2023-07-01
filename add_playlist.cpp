#include "add_playlist.h"
#include "ui_add_playlist.h"


add_playlist::add_playlist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_playlist)
{
    ui->setupUi(this);
}

add_playlist::~add_playlist()
{
    delete ui;
}

// Если жму "Отмена"
void add_playlist::on_buttonBox_rejected()
{
    close();
}

// Если жму "ОК"
void add_playlist::on_buttonBox_accepted()
{
    QString playlist_name = ui->lineEdit->text();
    if(DataBaseHandler::instance().addPlaylist(playlist_name))
    {
        close();
    }
}


#include "playlistwindow.h"
#include "ui_playlistwindow.h"

PlaylistWindow::PlaylistWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaylistWindow)
{
    ui->setupUi(this);
}

PlaylistWindow::~PlaylistWindow()
{
    delete ui;
}

void PlaylistWindow::setPlaylistName(const QString &playlistName)
{
    ui->playlistName->setText(playlistName);
}

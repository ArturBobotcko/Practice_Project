#include "playlistwindow.h"
#include "ui_playlistwindow.h"
#include "StyleHelper.h"

PlaylistWindow::PlaylistWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaylistWindow)
{
    ui->setupUi(this);
    setIntefaceStyle();
}

PlaylistWindow::~PlaylistWindow()
{
    delete ui;
}

void PlaylistWindow::setPlaylistName(const QString &playlistName)
{
    ui->playlistName->setText(playlistName);
}

void PlaylistWindow::setIntefaceStyle()
{
    StyleHelper::setAllButtonsStyle(ui);
    StyleHelper::setAllSlidersStyle(ui);
}

#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H

#include <QDialog>
#include <QMediaMetaData>

namespace Ui {
class PlaylistWindow;
}

class PlaylistWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistWindow(QWidget *parent = nullptr);
    ~PlaylistWindow();
    void setPlaylistName(const QString& playlistName);
    void setIntefaceStyle();
    void insertTracks();

private:
    Ui::PlaylistWindow *ui;

};

#endif // PLAYLISTWINDOW_H

#ifndef SELECTPLAYLIST_H
#define SELECTPLAYLIST_H

#include <QDialog>

namespace Ui {
class selectPlaylist;
}

class selectPlaylist : public QDialog
{
    Q_OBJECT

public:
    explicit selectPlaylist(QWidget *parent = nullptr);
    ~selectPlaylist();

    void setRowValues(const QStringList &newRowValues);

private slots:
    void on_pushButton_clicked();

private:
    Ui::selectPlaylist *ui;
    void fullComboBox();
    QStringList rowValues;
};

#endif // SELECTPLAYLIST_H

#include "add_playlist.h"
#include "ui_add_playlist.h"
#include "mainwindow.h"

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
    if (DataBaseHandler::instance().addPlaylist(playlist_name))
    {
        // Получение указателя на главную форму
        MainWindow* mainWindow = qobject_cast<MainWindow*>(parentWidget());

        // Проверка, что получен правильный указатель на главную форму
        if (mainWindow)
        {
            // Получение указателя на QTableWidget
            QTableWidget* tableWidget = mainWindow->findChild<QTableWidget*>("playlist_list");
            if (tableWidget)
            {
                // Получение количества строк в таблице
                int rowCount = tableWidget->rowCount();

                // Добавление новой строки
                tableWidget->insertRow(rowCount);

                // Установка названия плейлиста в ячейку
                QTableWidgetItem* item = new QTableWidgetItem(playlist_name);
                tableWidget->setItem(rowCount, 0, item);
            }
        }

        close();
    }
}


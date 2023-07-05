#include "StyleHelper.h"
#include "ui_mainwindow.h"
#include "ui_playlistwindow.h"

QString StyleHelper::setButtonStyle()
{
	return 
		"QPushButton {"
		"	border-radius: 50%;"
		"}"
		"QPushButton:pressed{"
		"	color: darker(@palette(midlight));"
		"}"
		;
}

void StyleHelper::setBackgroundStyle(Ui_MainWindow *ui)
{
    ui->centralwidget->setStyleSheet("background-color: #e8e8e8; ");
}

QString StyleHelper::setSliderStyle()
{
    return
        "QSlider {"
            "height: 4px;"
            "width: 100%;"
            "margin: 2px;"
            "border-radius: 2px;"
            "background-color: #888888;"
        "}"
        "QSlider::groove:horizontal {"
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #cccccc, stop:1 #ffffff);"
        "}"
        "QSlider::handle:horizontal {"
            "width: 10px;"
            "margin: -3px 0;"
            "border-radius: 5px;"
            "background-color: #ffffff;"
        "}"
        "QSlider::handle:horizontal:hover {"
            "background-color: #e6e6e6;"
        "}"
        "QSlider::sub-page:horizontal {"
            "background: #ffffff;"
        "}"
        "QSlider::add-page:horizontal {"
            "background: #cccccc;"
        "}"

        ;
}

void StyleHelper::setAllButtonsStyle(Ui_MainWindow* ui)
{
	ui->playBtn->setStyleSheet(setButtonStyle());
	ui->nextTrackBtn->setStyleSheet(setButtonStyle());
	ui->prevTrackBtn->setStyleSheet(setButtonStyle());
	ui->stopTrackBtn->setStyleSheet(setButtonStyle());
	ui->muteBtn->setStyleSheet(setButtonStyle());
    ui->mixButton->setStyleSheet(setButtonStyle());
    ui->repeatButton->setStyleSheet(setButtonStyle());
}

void StyleHelper::setAllButtonsStyle(Ui_PlaylistWindow *ui)
{
    ui->playBtn->setStyleSheet(setButtonStyle());
    ui->nextTrackBtn->setStyleSheet(setButtonStyle());
    ui->prevTrackBtn->setStyleSheet(setButtonStyle());
    ui->stopTrackBtn->setStyleSheet(setButtonStyle());
    ui->muteBtn->setStyleSheet(setButtonStyle());
    ui->mixButton->setStyleSheet(setButtonStyle());
    ui->repeatButton->setStyleSheet(setButtonStyle());
}

void StyleHelper::setAllSlidersStyle(Ui_MainWindow* ui)
{
    ui->trackSlider->setStyleSheet(setSliderStyle());
    ui->volumeSlider->setStyleSheet(setSliderStyle());
}

void StyleHelper::setAllSlidersStyle(Ui_PlaylistWindow *ui)
{
    ui->trackSlider->setStyleSheet(setSliderStyle());
    ui->volumeSlider->setStyleSheet(setSliderStyle());
}
QString StyleHelper::setplaylestsLabelStyle()
{
	return
		"QLabel {"
		"background-color:#616a6b;"
		"border: 1px solid #bbb;"
		"border-radius: 4px;"
		"color: white;"
		"font-family:Snell Roundhand;"
		"font-family:cursive;"
		"}"
		;
}
void StyleHelper::setLablesStyle(Ui_MainWindow* ui)
{
    ui->playlistsLabel->setStyleSheet("font-size: 20px; font-weight: bold; ");
    ui->playlistsLabel->setStyleSheet(setplaylestsLabelStyle());
}


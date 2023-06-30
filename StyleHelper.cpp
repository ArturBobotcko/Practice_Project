#include "StyleHelper.h"

QString StyleHelper::setButtonStyle()
{
	return 
		"QPushButton {"
		"	border-radius: 50%;"
		"}"
		"QPushButton:hover {"
		"	filter: brightness(130%);"
		"}"
		"QPushButton:pressed{"
		"	color: darker(@palette(midlight));"
		"}"
		;
}

void StyleHelper::setBackgroundStyle(Ui::MainWindow *ui)
{
	ui->centralwidget->setStyleSheet("background-color: rgb(230, 230, 250); ");
}

QString StyleHelper::setSliderStyle()
{
	return
		"QSlider::groove:horizontal {"
		"border: 1px solid #bbb;"
		"background: white;"
		"height: 10px;"
		"border-radius: 4px;"
		"}"
		"QSlider::sub-page:horizontal {"
		"background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,"
			"stop: 0 #66e, stop: 1 #bbf);"
		"background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,"
			"stop: 0 #bbf, stop: 1 #55f);"
		"border: 1px solid #777;"
		"height: 10px;"
		"border-radius: 4px;"
		"}"
		"QSlider::add-page:horizontal {"
		"background: #fff;"
		"border: 1px solid #777;"
		"height: 10px;"
		"border-radius: 4px;"
		"}"
		"QSlider::handle:horizontal{"
		"background: qlineargradient(x1 : 0, y1 : 0, x2 : 1, y2 : 1,"
			"stop : 0 #eee, stop:1 #ccc);"
		"border: 1px solid #777;"
		"width: 13px;"
		"margin - top: -2px;"
		"margin - bottom: -2px;"
		"border - radius: 4px;"
		"}"
		"QSlider::handle:horizontal:hover{"
		"background: qlineargradient(x1 : 0, y1 : 0, x2 : 1, y2 : 1,"
			"stop : 0 #fff, stop:1 #ddd);"
		"border: 1px solid #444;"
		"border - radius: 4px;"
		"}"
		"QSlider::sub - page:horizontal:disabled{"
		"background: #bbb;"
		"border-color: #999;"
		"}"
		"QSlider::add - page : horizontal : disabled{"
		"background: #eee;"
		"border - color: #999;"
		"}"
		"QSlider::handle : horizontal : disabled{"
		"background: #eee;"
		"border: 1px solid #aaa;"
		"border-radius: 4px;"
		"}"
		
		;
}

void StyleHelper::setAllButtonsStyle(Ui::MainWindow* ui)
{
	ui->playBtn->setStyleSheet(setButtonStyle());
	ui->nextTrackBtn->setStyleSheet(setButtonStyle());
	ui->prevTrackBtn->setStyleSheet(setButtonStyle());
	ui->stopTrackBtn->setStyleSheet(setButtonStyle());
	ui->muteBtn->setStyleSheet(setButtonStyle());
}

void StyleHelper::setAllSlidersStyle(Ui::MainWindow* ui)
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
void StyleHelper::setLablesStyle(Ui::MainWindow* ui)
{
	ui->playlestsLabel->setStyleSheet("font-size: 20px; font-weight: bold; ");
	ui->playlestsLabel->setStyleSheet(setplaylestsLabelStyle());
}


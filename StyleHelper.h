#pragma once

#include <QString>
#include "ui_MainWindow.h"

class StyleHelper
{
public:
	static QString setButtonStyle();
	static void setBackgroundStyle(Ui::MainWindow* ui);
	static QString setSliderStyle();
	static void setAllButtonsStyle(Ui::MainWindow* ui);
	static void setAllSlidersStyle(Ui::MainWindow* ui);
	static void setLablesStyle(Ui::MainWindow* ui);
	static QString setplaylestsLabelStyle();
};


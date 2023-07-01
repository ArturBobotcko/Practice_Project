#pragma once

#include <QString>

class Ui_MainWindow;

class StyleHelper
{
public:
    static QString setButtonStyle();
    static void setBackgroundStyle(Ui_MainWindow* ui);
	static QString setSliderStyle();
    static void setAllButtonsStyle(Ui_MainWindow* ui);
    static void setAllSlidersStyle(Ui_MainWindow* ui);
    static void setLablesStyle(Ui_MainWindow* ui);
	static QString setplaylestsLabelStyle();
};


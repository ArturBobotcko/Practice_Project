#pragma once

#include <QString>

class Ui_MainWindow;
class Ui_PlaylistWindow;

class StyleHelper
{
public:
    static QString setButtonStyle();
    static QString setSliderStyle();
    static QString setplaylestsLabelStyle();
    static void setBackgroundStyle(Ui_MainWindow* ui);
    static void setAllButtonsStyle(Ui_MainWindow* ui);
    static void setAllButtonsStyle(Ui_PlaylistWindow* ui);
    static void setAllSlidersStyle(Ui_MainWindow* ui);
    static void setAllSlidersStyle(Ui_PlaylistWindow* ui);
    static void setLablesStyle(Ui_MainWindow* ui);
};


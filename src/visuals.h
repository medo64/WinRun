#pragma once

#include <QString>
#include <QWidget>


class Visuals {

    public:
        static void setupTheme(bool darkMode);
        static bool isOSInDarkMode();
        static QIcon iconApp();

};

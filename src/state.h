#pragma once
#include "medo/config.h"

class State {

    public:

        static int lastWidth();
        static void setLastWidth(int newLastWidth);

        static int lastQuadrant();
        static void setLastQuadrant(int newLastQuadrant);

        static QStringList history();
        static void setHistory(QStringList newHistory);

};

#pragma once
#include "medo/config.h"

class State {

    public:

        static int lastWidth();
        static void setLastWidth(int newLastWidth);

        static QStringList history();
        static void setHistory(QStringList newHistory);

};

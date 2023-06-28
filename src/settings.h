#pragma once
#include <QKeySequence>
#include "medo/config.h"

class Settings {

    public:

        static bool forceDarkMode();
        static void setForceDarkMode(bool newForceDarkMode);
        static bool defaultForceDarkMode() { return false; }

        static QKeySequence hotkey();
        static void setHotkey(QKeySequence newHotkey);
        static QKeySequence defaultHotkey() {
            QKeySequence defaultHotkey { "Meta+R" };
            return  defaultHotkey;
        }

        static int historyCount();
        static void setHistoryCount(int newHistoryCount);
        static int defaultHistoryCount() { return 42; }

        static bool startEmpty();
        static void setStartEmpty(bool newStartEmpty);
        static bool defaultStartEmpty() { return false; }

};

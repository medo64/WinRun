#pragma once
#include <QKeySequence>
#include <QFileInfo>
#include <QString>

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

        static QString fileManager();
        static void setFileManager(QString newFileManager);
        static QString defaultFileManager() {  // TODO: Proper detection of file manager
            QFileInfo fileDolphin("/usr/bin/dolphin");
            if (fileDolphin.exists()) { return fileDolphin.filePath(); }
            QFileInfo fileNautilus("/usr/bin/nautilus");
            if (fileNautilus.exists()) { return fileNautilus.filePath(); }
            QFileInfo fileThunar("/usr/bin/thunar");
            if (fileThunar.exists()) { return fileThunar.filePath(); }
            return QString();  // give up
        }

};

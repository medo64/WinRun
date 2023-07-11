#include "medo/config.h"
#include "settings.h"


bool Settings::forceDarkMode() {
    return Config::read("ForceDarkMode", defaultForceDarkMode());
}

void Settings::setForceDarkMode(bool newForceDarkMode) {
    Config::write("ForceDarkMode", newForceDarkMode);
}


QKeySequence Settings::hotkey() {
    QString hotkeyText = Config::read("Hotkey", "");
    if (hotkeyText.compare("-") == 0) {
        return 0;  // explicit no shortcut
    } else if (hotkeyText.length() > 0) {
        QKeySequence hotkeyKeys = QKeySequence(hotkeyText, QKeySequence::PortableText);
        if (hotkeyKeys.count() > 0) { return QKeySequence { hotkeyKeys[0] }; } //return first key found
    }
    return defaultHotkey();
}

void Settings::setHotkey(QKeySequence newHotkey) {
    QString hotkeyText { newHotkey == 0 ? "-" : newHotkey.toString(QKeySequence::PortableText) };
    Config::write("Hotkey", hotkeyText);
}


int Settings::historyCount() {
    return Config::read("HistoryCount", defaultHistoryCount());
}

void Settings::setHistoryCount(int newHistoryCount) {
    Config::write("HistoryCount", newHistoryCount);
}


bool Settings::startEmpty() {
    return Config::read("StartEmpty", defaultStartEmpty());
}

void Settings::setStartEmpty(bool newStartEmpty) {
    Config::write("StartEmpty", newStartEmpty);
}


QString Settings::fileManager() {
    return Config::read("FileManager", defaultFileManager());
}

void Settings::setFileManager(QString newFileManager) {
    Config::write("FileManager", newFileManager);
}

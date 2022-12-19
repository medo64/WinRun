#include "medo/config.h"
#include "state.h"


int State::lastWidth() {
    return Config::stateRead("LastWidth", 0);
}

void State::setLastWidth(int newLastWidth) {
    Config::stateWrite("LastWidth", newLastWidth);
    Config::save();
}


QStringList State::history() {
    return Config::stateReadMany("History");
}

void State::setHistory(QStringList newHistory) {
    Config::stateWriteMany("History", newHistory);
    Config::save();
}

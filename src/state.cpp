#include "medo/config.h"
#include "state.h"


int State::lastWidth() {
    int value = Config::stateRead("LastWidth", 0);
    if (value < 0) { value = 0; }
    if (value > 16777215) { value = 16777215; }
    return value;
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


int State::lastQuadrant() {
    int value = Config::stateRead("LastQuadrant", 0);
    if ((value < 0) || (value > 9)) { value = 0; }
    return value;
}

void State::setLastQuadrant(int newLastQuadrant) {
    Config::stateWrite("LastQuadrant", newLastQuadrant);
    Config::save();
}

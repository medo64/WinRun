#include <QAbstractButton>
#include <QApplication>
#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QDir>
#include <QProcess>
#include <QStyle>
#include <QStyleFactory>
#include <QUrl>
#include "visuals.h"


/*!
 * \brief Sets application palette to dark
 */
void Visuals::setupTheme(bool darkMode) {
    if (darkMode) {
        qApp->setStyle(QStyleFactory::create("Fusion"));

        QPalette newPalette;
        newPalette.setColor(QPalette::Window,          QColor( 37,  37,  37));
        newPalette.setColor(QPalette::WindowText,      QColor(212, 212, 212));
        newPalette.setColor(QPalette::Base,            QColor( 60,  60,  60));
        newPalette.setColor(QPalette::AlternateBase,   QColor( 45,  45,  45));
        newPalette.setColor(QPalette::PlaceholderText, QColor(127, 127, 127));
        newPalette.setColor(QPalette::Text,            QColor(212, 212, 212));
        newPalette.setColor(QPalette::Button,          QColor( 45,  45,  45));
        newPalette.setColor(QPalette::ButtonText,      QColor(212, 212, 212));
        newPalette.setColor(QPalette::BrightText,      QColor(240, 240, 240));
        newPalette.setColor(QPalette::Highlight,       QColor( 38,  79, 120));
        newPalette.setColor(QPalette::HighlightedText, QColor(240, 240, 240));

        newPalette.setColor(QPalette::Link,            QColor(182, 208, 250));
        newPalette.setColor(QPalette::LinkVisited,     QColor(222, 208, 250));

        newPalette.setColor(QPalette::Light,           QColor( 60,  60,  60));
        newPalette.setColor(QPalette::Midlight,        QColor( 52,  52,  52));
        newPalette.setColor(QPalette::Dark,            QColor( 30,  30,  30) );
        newPalette.setColor(QPalette::Mid,             QColor( 37,  37,  37));
        newPalette.setColor(QPalette::Shadow,          QColor( 0,    0,   0));

        newPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));

        qApp->setPalette(newPalette);
    } else {
        qApp->setPalette(qApp->style()->standardPalette());
        qApp->setStyle(QStyleFactory::create("WindowsVista"));
    }
}

/*!
 * \brief Checks if OS is in dark mode.
 * On Linux returns false always.
 */
bool Visuals::isOSInDarkMode() {
#if defined(Q_OS_WIN)
    return false; //TODO
#else
    return false;
#endif
}


/*!
 * \brief Returns application icon.
 */
QIcon Visuals::iconApp() {
    QIcon icon;
#ifndef QT_DEBUG
    icon.addFile(":icons/16x16/app.png", QSize(16, 16));
    icon.addFile(":icons/24x24/app.png", QSize(24, 24));
    icon.addFile(":icons/32x32/app.png", QSize(32, 32));
    icon.addFile(":icons/48x48/app.png", QSize(48, 48));
    icon.addFile(":icons/64x64/app.png", QSize(64, 64));
    icon.addFile(":icons/96x96/app.png", QSize(96, 96));
    icon.addFile(":icons/128x128/app.png", QSize(128, 128));
#else
    icon.addFile(":icons/16x16/appMono.png", QSize(16, 16));
    icon.addFile(":icons/24x24/appMono.png", QSize(24, 24));
    icon.addFile(":icons/32x32/appMono.png", QSize(32, 32));
    icon.addFile(":icons/48x48/appMono.png", QSize(48, 48));
    icon.addFile(":icons/64x64/appMono.png", QSize(64, 64));
    icon.addFile(":icons/96x96/appMono.png", QSize(96, 96));
    icon.addFile(":icons/128x128/appMono.png", QSize(128, 128));
#endif
    return icon;
}

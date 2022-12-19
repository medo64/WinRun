#include <QCommandLineParser>
#include "medo/appsetupmutex.h"
#include "medo/config.h"
#include "medo/singleinstance.h"
#include "ui/mainwindow.h"
#include "settings.h"


int main(int argc, char* argv[]) {
    QCoreApplication::setApplicationName(APP_PRODUCT);
    QCoreApplication::setOrganizationName(APP_COMPANY);
    QCoreApplication::setApplicationVersion(APP_VERSION);

    QCoreApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    AppSetupMutex appMutex("Medo_WinRun");
    QApplication app(argc, argv);

    QCommandLineParser cli;
    cli.setApplicationDescription(APP_DESCRIPTION);
    cli.addHelpOption();
    cli.addVersionOption();
    cli.process(app);

    QApplication::setQuitOnLastWindowClosed(false);

    if (!SingleInstance::attach()) {
        return static_cast<int>(0x80004004); //exit immediately if another instance is running
    }

    MainWindow w;
    w.show();

    return app.exec();
}
